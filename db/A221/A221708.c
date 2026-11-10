/*
 * A221708  Records in A216377.
 * https://oeis.org/A221708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index m of the n-th strict record (left-to-right maximum)
 *            of A216377(m) = leading digit of m! in base m, m >= 2. Found by
 *            scanning m upward, using prefix sums of logarithms
 * Range:     u8 n <= 21, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Validated: standard, pass, 66 terms through n = 147 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static idx_t *A221708_found = 0;
static size_t A221708_cnt = 0, A221708_cap = 0;
static idx_t A221708_m = 1;          /* last m processed */
static long double A221708_S = 0.0L; /* ln(m!) */
static long long A221708_best = 0;

static long long A221708_digit(idx_t m, long double S)
{
    long double lnm = logl((long double)m);
    long double x = S / lnm;
    long double k = floorl(x);
    long double frac = x - k;
    long double val = expl(frac * lnm);
    long long d = (long long)(val + 1e-9L);
    if (d < 1) d = 1;
    if (d >= (long long)m) d = (long long)m - 1;
    return d;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A221708_cnt < (size_t)n) {
        idx_t m = ++A221708_m;
        A221708_S += logl((long double)m);
        if (m < 2) continue;
        long long d = A221708_digit(m, A221708_S);
        if (d > A221708_best) {
            A221708_best = d;
            if (A221708_cnt == A221708_cap) {
                A221708_cap = A221708_cap ? A221708_cap * 2 : 64;
                A221708_found = (idx_t *)realloc(A221708_found, A221708_cap * sizeof(idx_t));
            }
            A221708_found[A221708_cnt++] = m;
        }
    }
    return (seq_t)A221708_found[n - 1];
}
