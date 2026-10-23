/*
 * A138797  Least possible T(k) with T(k)-T(j)=n, where T(i)>0 are the
 *          triangular numbers A000217.
 * https://oeis.org/A138797
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least triangular number T(k) such that T(k)-T(j)=n (with
 *            j>=1)
 * Range:     u8 n <= 22, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The calculation uses a factorisation of 2n = (k-j)*(k+j+1). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* values below the offset are undefined */
    seq_t M = (seq_t)n * 2;              /* 2n, used as product (k-j)*(k+j+1) */
    seq_t best = 0;
    bool found = false;

    for (idx_t d = 1; ; ++d) {
        seq_t d_seq = d;
        seq_t d2 = d_seq * d_seq;
        if (d2 > M) break;               /* stop when d > sqrt(2n) */
        if (M % d_seq != 0) continue;    /* d must divide M */
        seq_t s = M / d_seq;             /* the complementary factor (k+j+1) */
        if (s <= d_seq) continue;        /* we need k-j < k+j+1 => d < s */

        seq_t diff = s - d_seq;
        if (diff < (seq_t)3) continue;   /* requires j >= 1 => s - d >= 3 */

        seq_t sum = s + d_seq;
        if ((sum & 1) == 0) continue;    /* (s + d) must be odd for integer k */

        seq_t k = (sum - 1) / 2;         /* k = (s + d -1)/2 */
        seq_t tk = k * (k + 1) / 2;       /* T(k) */
        if (!found || tk < best) {
            best = tk;
            found = true;
        }
    }
    return best;                          /* guaranteed to be found for n >= 2 */
}
