/*
 * A061703  G.f.: 2*x*(2-2*x-3*x^2+2*x^3)/((1-3*x-x^2+x^3)*(1-x)).
 * https://oeis.org/A061703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) + a(n-4) for n≥5, with
 *            a(0)=0, a(1)=4, a(2)=12, a(3)=34, a(4)=108
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;
static size_t cap = 0;
static size_t last = 0;

/* Ensure space for indices up to n (inclusive). */
static void ensure_cap(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        cap = new_cap;
    }
}

/* Initialize the first five sequence values. */
static void init_first(void)
{
    if (last == 0) {
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)4;
        seq[2] = (seq_t)12;
        seq[3] = (seq_t)34;
        seq[4] = (seq_t)108;
        last = 4;
    }
}

/* Compute terms up to n. */
static void compute_until(idx_t n)
{
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = seq[i-1] * (seq_t)4;
        term -= (seq[i-2] + seq[i-3]) * (seq_t)2;
        term += seq[i-4];
        seq[i] = term;
    }
    last = n;
}

seq_t a(idx_t n)
{
    ensure_cap(n);
    init_first();
    if (n <= last) return seq[(size_t)n];
    compute_until(n);
    return seq[(size_t)n];
}
