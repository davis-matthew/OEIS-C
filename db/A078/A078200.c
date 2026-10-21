/*
 * A078200  a(n) = A078199(n)/n; i.e., smallest k such that frequency of each
 *          occurring digit in k*n is the same.
 * https://oeis.org/A078200
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078199(n) / n, i.e. the smallest integer k such that the
 *            decimal * representation of k*n has all (occurring) digits
 *            appearing with the same frequency
 * Range:     u8 none, u16 n <= 201, u32 n <= 201, u64 n <= 201, u128 n <= 201,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not needed for the official offset */
    seq_t base = (seq_t)n;

    idx_t k = 1;
    for (;; ++k) {
        seq_t prod = base * (seq_t)k;

        int cnt[10] = {0};
        seq_t tmp = prod;
        if (tmp == 0) {
            cnt[0] = 1;                 /* only possible for prod==0, which won't happen here */
        } else {
            while (tmp > 0) {
                int d = (int)(tmp % (seq_t)10);
                ++cnt[d];
                tmp /= (seq_t)10;
            }
        }

        int freq = -1;
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] > 0) {
                if (freq == -1) {
                    freq = cnt[d];
                } else if (cnt[d] != freq) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) return (seq_t)k;
    }
}
