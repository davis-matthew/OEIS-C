/*
 * A049986  a(n) is the number of arithmetic progressions of 4 or more
 *          positive integers, strictly increasing with sum n.
 * https://oeis.org/A049986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of arithmetic progressions of * positive
 *            integers, strictly increasing, of length at least 4, * whose
 *            terms sum to n. * Implementation: iterate over all divisors k of
 *            2n (k ≥ 4). * For each k, check the divisibility condition and
 *            the * constraints on the common difference d...
 * Range:     u8 n <= 839, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: floor integer square root for an unsigned integer. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    /* Edge: for small n the count is zero. */
    if (n < 4) return 0;

    seq_t total = 0;
    /* 2n as an unsigned integer; safe for n up to 10000. */
    idx_t val64 = (idx_t)n * 2;

    idx_t int_sqrt = floor_isqrt_u64(val64);
    for (idx_t i = 1; i <= int_sqrt; i++) {
        if (val64 % i != 0) continue;

        /* First divisor k1 = i */
        idx_t ks[2] = { i, val64 / i };
        for (int t = 0; t < 2; t++) {
            idx_t k = ks[t];
            if (k < 4) continue;            /* length must be at least 4 */
            if (val64 % k != 0) continue;   /* safety; should always hold */

            seq_t T = ((seq_t)val64) / (seq_t)k;   /* T = 2n / k */
            if (T < (seq_t)k + 1) continue;        /* requires T >= k+1 */

            seq_t Dmax = (T - 2) / ((seq_t)k - 1); /* maximum allowed d */
            if (Dmax == 0) continue;

            /* Parity considerations */
            if (((k - 1) & 1) == 0) { /* k-1 even */
                if ((T & 1) != 0) continue;       /* necessary T even */
                total += Dmax;                     /* all d are valid */
            } else { /* k-1 odd */
                seq_t parity = T & 1;              /* 0 for even, 1 for odd */
                seq_t first = (parity == 1) ? 1 : 2;
                if (Dmax < first) continue;
                total += ((Dmax - first) / 2) + 1;
            }
        }
    }
    return total;
}
