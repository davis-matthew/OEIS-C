/*
 * A136764  a(n) = leading digit of n! in base 14.
 * https://oeis.org/A136764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = leading digit of n! in base 14. Computed by using
 *            logarithms: a(n) = floor(10^( fractional_part( log10(n!) -
 *            floor(log10(n!)/log10(14))*log10(14) ) )). The value is always
 *            in [1,13]
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static long double *cum = NULL;   /* cumulative sums of log10(i) up to current max index */
static idx_t cap = 0;             /* allocated size of cum array (max index + 1) */
static const long double log10_14 = log10l(14.0L);

/* Ensure cum[0..n] are computed */
static void ensure_capacity(idx_t n) {
    if (n < cap) return;
    idx_t new_cap = n + 1;
    long double *new_cum = (long double*)realloc(cum, new_cap * sizeof(long double));
    if (!new_cum) exit(1);
    if (!cum) new_cum[0] = 0.0L;
    for (idx_t i = cap; i < new_cap; ++i) {
        if (i == 0) new_cum[i] = 0.0L;
        else new_cum[i] = new_cum[i-1] + log10l((long double)i);
    }
    cum = new_cum;
    cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* 0! = 1 → leading digit 1 */
    ensure_capacity(n);
    long double x = cum[n];                        /* log10(n!) */
    long double k = floorl(x / log10_14);          /* floor(log base 14) */
    long double f = x - k * log10_14;              /* fractional part times log10 */
    long double val = powl(10.0L, f);              /* 10^(fractional part) ∈ [1,14) */
    /* floor to integer, adding tiny epsilon to guard against 9.999999... */
    seq_t d = (seq_t)((int)floorl(val + 1e-12L));
    return d;
}
