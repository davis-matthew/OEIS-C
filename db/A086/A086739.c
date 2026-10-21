/*
 * A086739  A000041(n)-A000010(n).
 * https://oeis.org/A086739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - A000010(n) (partition number minus Euler's
 *            totient)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for partition and phi values */
    static seq_t *part = NULL;
    static idx_t part_size = 0;
    static seq_t *phi  = NULL;
    static idx_t  phi_size  = 0;

    /* expand caches if needed */
    if (n + 1 > part_size) {
        idx_t new_sz = n + 1;
        part = (seq_t *)realloc(part, new_sz * sizeof(seq_t));
        phi  = (seq_t *)realloc(phi , new_sz * sizeof(seq_t));
        part_size = new_sz;
        phi_size  = new_sz;

        /* init phi array */
        for (idx_t i = 0; i < new_sz; ++i)
            phi[i] = (seq_t)i;

        /* sieve to compute phi up to n */
        for (idx_t i = 2; i <= n; ++i) {
            if (phi[i] == (seq_t)i) {            /* i is prime */
                for (idx_t j = i; j <= n; j += i)
                    phi[j] = phi[j] - phi[j] / (seq_t)i;
            }
        }

        /* partition numbers using generalized pentagonal recurrence */
        part[0] = 1;
        for (idx_t m = 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > m) break;
                if (k & 1)          /* odd k -> + */
                    sum += part[m - g1];
                else                /* even k -> - */
                    sum -= part[m - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > m) continue;
                if (k & 1)
                    sum += part[m - g2];
                else
                    sum -= part[m - g2];
            }
            part[m] = sum;
        }
    }

    /* final difference */
    return part[n] - phi[n];
}
