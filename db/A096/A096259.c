/*
 * A096259  Longest period of an abstract version of the game of Go on a 1 X n
 *          board.
 * https://oeis.org/A096259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n=1, 2 for n=2, 6 for n=3, and for n >= 4: Let n_0
 *            = n. For k >= 1: let h_{k-1} = v2(n_{k-1}) (highest power of two
 *            dividing n_{k-1}) n_k = n - floor(n_{k-1}/2^{h_{k-1}+1}) - 1
 *            Stop when n_m is a power of two (i.e. n_m / 2^{h_m} = 1). Define
 *            p = m mod 2. Then a(n) = n * 2^p * ( sum_{...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   count_trailing_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_trailing_zeros.h"

seq_t a(idx_t n) {
    /* handle small n directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    idx_t cur = n;           /* current n_k */
    seq_t S = 0;             /* accumulated sum */
    int m = 0;               /* index of last step */

    while (1) {
        /* compute h_k = v2(cur) */
        unsigned h = count_trailing_zeros((uint64_t)cur);

        /* add sum_{i=0}^{h} (cur >> i) */
        seq_t local = 0;
        uint64_t tmp = (uint64_t)cur;
        for (unsigned i = 0; i <= h; ++i) {
            local += (seq_t)tmp;
            tmp >>= 1;
        }
        S += local;

        /* stop if cur is power of two */
        if ((cur & (cur - 1)) == 0)
            break;

        /* compute next n_k */
        uint64_t div_part = (uint64_t)cur >> (h + 1);   /* floor(cur / 2^{h+1}) */
        cur = n - div_part - 1;

        ++m;
    }

    /* compute final result */
    seq_t power = (m & 1) ? (seq_t)2 : (seq_t)1;  /* 2^p */
    seq_t result = (seq_t)n * power * (S - (seq_t)1);
    return result;
}
