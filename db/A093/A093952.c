/*
 * A093952  Partition number A000041(n) mod n.
 * https://oeis.org/A093952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) mod n * where p(n) is the partition number computed
 *            by the generalized * pentagonal number theorem. All arithmetic
 *            is performed modulo n
 * Range:     u8 n <= 270, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* not used, but safe */

    /* Allocate array to hold partition values modulo n */
    idx_t limit = n;
    seq_t *p = (seq_t *)malloc(sizeof(seq_t) * (limit + 1));
    if (!p) return (seq_t)0;                     /* allocation failure */

    /* Base case: p(0) = 1 (mod n) */
    p[0] = ((seq_t)1) % ((seq_t)n);

    /* Compute p(m) for m = 1 .. n, all modulo n */
    for (idx_t m = 1; m <= limit; ++m) {
        uint64_t sum = 0;                         /* use 64‑bit temporary */
        uint64_t mod = (uint64_t)n;
        int sign = 1;                             /* +1, -1 alternates */

        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;

            uint64_t add = (uint64_t)p[m - pent1];
            if (sign > 0) {
                sum += add;
                if (sum >= mod) sum -= mod;
            } else {
                sum += mod - add;
                if (sum >= mod) sum -= mod;
            }

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= m) {
                add = (uint64_t)p[m - pent2];
                if (sign > 0) {
                    sum += add;
                    if (sum >= mod) sum -= mod;
                } else {
                    sum += mod - add;
                    if (sum >= mod) sum -= mod;
                }
            }
            sign = -sign;                        /* alternate sign */
        }
        p[m] = (seq_t)sum;
    }

    seq_t result = p[limit];
    free(p);
    return result;
}
