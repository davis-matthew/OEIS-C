/*
 * A198680  Multiples of 3 whose sum of base-3 digits are also multiples of 3.
 * https://oeis.org/A198680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 3 * A079498(n) (OEIS A198680)
 * Range:     u8 n <= 29, u16 n <= 7282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;                 /* a(1) = 0 */

    /* pre‑compute powers of 3 : pow3[i] = 3^i  (unsigned 64‑bit) */
    static uint64_t pow3[65];
    static int pow3_done = 0;
    if (!pow3_done) {
        pow3[0] = 1ULL;
        for (int i = 1; i < 65; ++i) pow3[i] = pow3[i-1] * 3ULL;
        pow3_done = 1;
    }

    /* find minimal L >= 2 with 3^(L-1) >= n   (cum(L) = 3^(L-1)) */
    int L = 2;
    while (pow3[L-1] < n) ++L;

    /* n-th element of A079498: index inside length‑L block */
    uint64_t rank = n - pow3[L-2] - 1ULL;        /* 0‑based within block */

    seq_t b = (seq_t)0;                          /* value of A079498(n) */
    int cur_sum = 0;                             /* modulo 3 sum so far */

    for (int pos = L - 1; pos >= 0; --pos) {
        int start = (pos == L - 1) ? 1 : 0;      /* leading digit != 0 */
        for (int d = start; d < 3; ++d) {
            int new_sum = (cur_sum + d) % 3;
            int target  = (3 - new_sum) % 3;
            int r = pos;                         /* remaining positions */
            uint64_t cnt = 0;
            if (r == 0) {
                cnt = (target == 0) ? 1ULL : 0ULL;
            } else {
                cnt = pow3[r-1];
            }
            if (rank < cnt) {                    /* choose this digit */
                b = (b * 3) + d;
                cur_sum = new_sum;
                break;
            }
            rank -= cnt;
        }
    }

    /* multiply by 3 to get the required multiple of 3 */
    return b * (seq_t)3;
}
