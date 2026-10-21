/*
 * A209313  Total number of parts k in all partitions of n such that k does
 *          not divide n.
 * https://oeis.org/A209313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006128(n) – A089251(n)
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 106, u64 n <= 360,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc */

seq_t a(idx_t n)
{
    /* P[i] holds p(i) – the number of partitions of i (p(0)=1) */
    static seq_t *P = NULL;
    static idx_t P_max = 0;   /* largest i for which P[i] is valid */

    if (n == 0)
        return 0; /* sequence starts at offset 1 */

    /* expand the P array to cover n if needed */
    if (n > P_max) {
        size_t new_len = (size_t)(n + 1);

        /* reallocate the partition table */
        seq_t *tmp = (seq_t *)realloc(P, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* memory failure */
        P = tmp;

        /* initialise base case if we are starting from scratch */
        if (P_max == 0)
            P[0] = (seq_t)1;

        /* compute partition numbers for new indices */
        for (idx_t i = P_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i)
                    break;
                seq_t val = P[i - pent1];
                if (k & 1)
                    sum += val;
                else
                    sum -= val;

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i)
                    continue;
                val = P[i - pent2];
                if (k & 1)
                    sum += val;
                else
                    sum -= val;
            }
            P[i] = sum;
        }
        P_max = n;
    }

    /* total number of parts over all partitions of n */
    seq_t total_parts = 0;
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t t = n - k; ; t -= k) {
            total_parts += P[t];
            if (t < k)
                break;
        }
    }

    /* number of parts whose size divides n */
    seq_t div_parts = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            for (idx_t t = n - d; ; t -= d) {
                div_parts += P[t];
                if (t < d)
                    break;
            }
            idx_t d2 = n / d;
            if (d2 != d) {
                for (idx_t t = n - d2; ; t -= d2) {
                    div_parts += P[t];
                    if (t < d2)
                        break;
                }
            }
        }
    }

    return total_parts - div_parts;  /* a(n) */
}
