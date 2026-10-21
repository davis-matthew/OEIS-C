/*
 * A214748  Least m>0 such that n-m divides (2*n-1)!!+m.
 * https://oeis.org/A214748
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ m>0 | (n-m) divides (2*n-1)!! + m } (search)
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;        /* sequence undefined for n<2 */

    /* we need remainders modulo d = n-m, 1 <= d <= n-1 */
    idx_t nn = n;
    /* allocate array of remainders, index 0 unused */
    uint64_t *rem = (uint64_t*)malloc((size_t)nn * sizeof(uint64_t));
    if (!rem) return (seq_t)0;         /* memory error */

    for (idx_t d = 1; d < nn; ++d) rem[d] = 1;

    /* compute (2*n-1)!! modulo each d efficiently */
    for (idx_t odd = 1; odd <= 2*nn - 1; odd += 2) {
        for (idx_t d = 1; d < nn; ++d) {
            if (odd % d == 0) {       /* product becomes 0 modulo d */
                rem[d] = 0;
                continue;
            }
            /* rem[d] < d <= 10000, odd <= 20000, product fits in 64 bits */
            uint64_t prod = (rem[d] * (uint64_t)odd) % (uint64_t)d;
            rem[d] = prod;
        }
    }

    /* find the least m>0 such that (n-m) divides (2*n-1)!! + m */
    seq_t answer = (seq_t)0;
    for (idx_t m = 1; m < nn; ++m) {
        idx_t d = nn - m;
        if ( (rem[d] + m) % d == 0 ) {
            answer = (seq_t)m;
            break;
        }
    }

    free(rem);
    return answer;
}
