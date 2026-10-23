/*
 * A123579  The Kruskal-Macaulay function M_3(n).
 * https://oeis.org/A123579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A123579: Kruskal-Macaulay function M3(n)
 * Formula:  Find the unique representation
 * n = C(k3,3) + C(k2,2) + C(k1,1) with k3 > k2 > k1 >= 1.
 * Then   a(n) = C(k3-1,2) + C(k2-1,1) + C(k1-1,0).
 */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t rem = (seq_t)n;
    idx_t k3 = 0, k2 = 0, k1 = 0;

    /* find largest k3 with C(k3,3) <= rem  (k3 >= 3) */
    for (k3 = 3; ; ++k3) {
        seq_t c = binomial(k3, 3);
        if (c > rem) {
            --k3;
            break;
        }
    }
    rem -= binomial(k3, 3);

    /* find largest k2 < k3 with C(k2,2) <= rem  (k2 >= 2) */
    if (rem > 0) {
        for (k2 = (k3 > 1 ? k3 - 1 : 1); k2 >= 2; --k2) {
            seq_t c = binomial(k2, 2);
            if (c <= rem) {
                rem -= c;
                break;
            }
        }
        if (k2 < 2) k2 = 0;          /* not found */
    }

    /* find largest k1 < k2 with C(k1,1) <= rem  (k1 >= 1) */
    if (rem > 0) {
        for (k1 = (k2 > 0 ? k2 - 1 : 1); k1 >= 1; --k1) {
            seq_t c = binomial(k1, 1);
            if (c <= rem) {
                rem -= c;
                break;
            }
        }
        if (k1 < 1) k1 = 0;          /* not found */
    }

    /* compute the M3 value */
    seq_t result = binomial(k3 - 1, 2);
    if (k2 > 0) result += binomial(k2 - 1, 1);
    if (k1 > 0) result += 1;           /* binomial(..,0) == 1 */

    return result;
}
