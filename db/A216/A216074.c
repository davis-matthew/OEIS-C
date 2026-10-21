/*
 * A216074  Number of ways in which a four-player Old Maid match lasts for
 *          exactly n games until a player has been trolled exactly three
 *          times.
 * https://oeis.org/A216074
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4 * C(n-1,2) * ways(n-3) * 4^(9-n)  for 3 ≤ n ≤ 9
 * where ways(L) counts strings of length L over 3 symbols with each symbol
 * appearing at most twice.  For n < 3 or n > 9, a(n) = 0.
 */
#include <stddef.h>

static const seq_t pow4[7] = {1u, 4u, 16u, 64u, 256u, 1024u, 4096u};      /* 4^k for k=0..6   */
static const seq_t ways[7] = {1u, 3u, 9u, 24u, 54u, 90u, 90u};            /* ways(L) for L=0..6 */

seq_t a(idx_t n)
{
    /* return 0 for indices outside the defined range */
    if (n < 3 || n > 9) {
        return (seq_t)0;
    }

    /* L = n-3 : number of positions after the first two occurrences of the warned digit */
    idx_t L = n - 3;                     /* 0 ≤ L ≤ 6 */
    seq_t waysL = ways[L];               /* ways for the remaining positions */

    /* binom = C(n-1, 2) = (n-1)*(n-2)/2  */
    seq_t nm1 = (seq_t)(n - 1);
    seq_t nm2 = (seq_t)(n - 2);
    seq_t binom = (nm1 * nm2) / (seq_t)2;

    /* pow4 = 4^(9-n), index 9-n ranges from 6 down to 0 */
    seq_t pow_val = pow4[9 - n];

    /* a(n) = 4 * binom * waysL * pow_val */
    seq_t res = (seq_t)4;
    res *= binom;
    res *= waysL;
    res *= pow_val;

    return res;
}
