/*
 * A097923  Expansion of (1+x^20)/((1-x)*(1-x^3)*(1-x^5)).
 * https://oeis.org/A097923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) + (n>=20 ? b(n-20) : 0); where b(n) = # of triples
 *            (i,j,k) with i,j,k≥0 and i+3j+5k = n. b(n) satisfies the
 *            recurrence b(0..8) – computed by direct enumeration, b(n) =
 *            b(n‑1) + b(n‑3) – b(n‑4) + b(n‑5) – b(n‑6) – b(n‑8) + b(n‑9) for
 *            n≥9
 * Range:     u8 n <= 66, u16 n <= 996, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *b = NULL;          /* array of coefficients b(n) */
static size_t computed = 0;      /* number of elements stored in b */
static size_t capacity = 0;      /* allocated size of b */

static void ensure_b(size_t upto)
{
    /* Allocate or enlarge the array if necessary */
    if (b == NULL || capacity < upto + 1) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap < upto + 1) new_cap = upto + 1;
        b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute missing values up to 'upto' */
    for (size_t n = computed; n <= upto; ++n) {
        if (n <= 8) {
            /* Direct enumeration: number of (i,j,k>=0) with i+3j+5k = n */
            seq_t cnt = 0;
            for (size_t k = 0; k <= n / 5; ++k) {
                size_t rem = n - 5 * k;
                cnt += (rem / 3) + 1; /* j from 0 to rem/3, i fills the rest */
            }
            b[n] = cnt;
        } else {
            seq_t val = 0;
            if (n >= 1) val += b[n - 1];
            if (n >= 3) val += b[n - 3];
            if (n >= 4) val -= b[n - 4];
            if (n >= 5) val += b[n - 5];
            if (n >= 6) val -= b[n - 6];
            if (n >= 8) val -= b[n - 8];
            if (n >= 9) val += b[n - 9];
            b[n] = val;
        }
    }
    computed = upto + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* safe cast: idx_t is unsigned */
    ensure_b(idx);                   /* ensure b[0..idx] are computed */

    seq_t result = b[idx];
    if (idx >= 20) result += b[idx - 20];
    return result;
}
