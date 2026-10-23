/*
 * A180692  12^a(n) is smallest power of 12 beginning with n.
 * https://oeis.org/A180692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest k ≥ 0 such that the decimal expansion of
 *            12^k starts with the decimal digits of n. The algorithm uses the
 *            leading‑digit property of powers of 12: the fractional part of
 *            k·log10(12) determines the leading digits
 * Range:     u8 n <= 82, u16 n <= 9029, u32 n <= 9029, u64 n <= 9029,
 *            u128 n <= 9029, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

#define LOG12 log10l((long double)12.0L)

seq_t a(idx_t n) {
    /* Handle the special case a(1)=0 directly */
    if (n == 1) return (seq_t)0;

    /* Compute the number of decimal digits of n (L) */
    int L = 0;
    uint64_t tmp = n;
    while (tmp) {
        ++L;
        tmp /= 10;
    }

    /* Pre‑compute the logarithmic bounds for n */
    long double n_ld   = (long double)n;
    long double n1_ld  = (long double)(n + 1);
    long double lower  = log10l(n_ld)  - (long double)(L - 1);  /* = log10(n/10^(L-1)) */
    long double upper  = log10l(n1_ld) - (long double)(L - 1);  /* = log10((n+1)/10^(L-1)) */

    /* The fractional part of k·log10(12) */
    long double alpha  = LOG12;
    long double frac   = fmodl(alpha, 1.0L);   /* initial for k = 1 */

    for (idx_t k = 1;; ++k) {
        /* Compute the first L digits of 12^k by lifting the fractional part */
        long double leading_val = powl(10.0L, frac + (long double)(L - 1));
        uint64_t leading_digits = (uint64_t)(leading_val + 1e-12L);  /* floor */

        if (leading_digits == n) return (seq_t)k;

        /* Advance to the next exponent */
        frac += alpha;
        if (frac >= 1.0L) {
            frac -= floorl(frac);   /* keep only fractional part */
        }
    }
}
