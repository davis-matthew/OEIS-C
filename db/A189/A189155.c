/*
 * A189155  Number of nX3 binary arrays without the pattern 0 0 1 1
 *          diagonally, vertically or horizontally.
 * https://oeis.org/A189155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (f(n))³, where f(n) is the number of binary strings of
 *            length n avoiding the substring 0011. f(0)=1, f(1)=2, f(2)=4,
 *            f(3)=8 and f(n) = f(n-1)+f(n-2)+f(n-3)+f(n-4) for n≥4 (automaton
 *            with 4 states)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* DP states:
       c0: strings ending in state 0 (suffix "" ),
       c1: ending in state 1 (suffix "0" ),
       c2: ending in state 2 (suffix "00"),
       c3: ending in state 3 (suffix "001") ;
       state 4 (suffix "0011") is forbidden and never counted. */
    seq_t c0 = 1, c1 = 0, c2 = 0, c3 = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t nc0 = c0 + c1;      /* transitions to state 0 */
        seq_t nc1 = c0 + c3;      /* transitions to state 1 */
        seq_t nc2 = c1 + c2;      /* transitions to state 2 */
        seq_t nc3 = c2;           /* transitions to state 3 */
        c0 = nc0; c1 = nc1; c2 = nc2; c3 = nc3;
    }
    seq_t f = c0 + c1 + c2 + c3;   /* total number of strings of length n avoiding 0011 */
    return f * f * f;              /* each of the 3 columns is independent */
}
