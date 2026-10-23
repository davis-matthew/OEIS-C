/*
 * A182492  Expansion of 1 - x - (1 - sqrt(1 + 4*x^4)) / (2*x) in powers of x.
 * https://oeis.org/A182492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(4k) = 0 for k>0 (a(0)=1) a(4k+1) = 0 for k>=0 (a(1)=-1)
 *            a(4k+2) = 0 a(4k+3) = (-1)^k * Catalan(k) with Catalan(0)=1 and
 *            Catalan(k) = Catalan(k-1) * (4*k-2)/(k+1) (k≥1)
 * Range:     i8 n <= 26, i16 n <= 46, i32 n <= 82, i64 n <= 146,
 *            i128 n <= 278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------- Catalan numbers cache ------------------------------- */
static seq_t *catalan = NULL;
static size_t catalan_cap = 0;

/* Ensure the cache holds indices 0 … need-1 */
static void ensure_catalan_capacity(size_t need)
{
    if (need <= catalan_cap)
        return;
    /* grow at least twice, but not less than needed */
    size_t new_cap = (catalan_cap == 0) ? 1 : catalan_cap * 2;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(catalan, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                 /* allocation failed */
    catalan = tmp;

    /* compute missing Catalan values */
    for (size_t i = catalan_cap; i < new_cap; ++i) {
        if (i == 0) {
            catalan[0] = (seq_t)1;
        } else {
            seq_t prev = catalan[i-1];
            seq_t mult = (seq_t)(4 * i - 2); /* 4*i-2 */
            seq_t denom = (seq_t)i + 1;
            catalan[i] = (prev * mult) / denom; /* exact division */
        }
    }
    catalan_cap = new_cap;
}

/* Return Catalan(k), extending cache as needed. */
static seq_t get_catalan(idx_t k)
{
    size_t need = (size_t)k + 1;
    ensure_catalan_capacity(need);
    return catalan[(size_t)k];
}

/* ----------------- Sequence A182492 ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    /* Only indices of the form 4k+3 can be non‑zero */
    if ((n & 3) != 3) return (seq_t)0;

    idx_t k = (n - 3) >> 2;     /* (n-3)/4 */
    seq_t cat = get_catalan(k);
    return ((k & 1) == 0) ? cat : -cat;
}
