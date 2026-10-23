/*
 * A179426  Number of ways to place 6 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n^2*(n^10-135*n^8+7525*n^6-217665*n^4+3289354*n^2-20949480)/720
 *            for n >= 7; a(6) = 10596; a(n) = 0 for n < 6
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 11, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small n */
    if (n < 6) {
        return 0;
    }
    if (n == 6) {
        return (seq_t)10596;
    }

    /* Intermediate calculations use unsigned __int128 to avoid overflow
       on uint64_t builds where the result still fits in 64 bits.  */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;         /* n^2 */
    unsigned __int128 n4   = n2 * n2;         /* n^4 */
    unsigned __int128 n6   = n4 * n2;         /* n^6 */
    unsigned __int128 n8   = n4 * n4;         /* n^8 */
    unsigned __int128 n10  = n8 * n2;         /* n^10 */

    unsigned __int128 poly = n10
                          - (unsigned __int128)135 * n8
                          + (unsigned __int128)7525 * n6
                          - (unsigned __int128)217665 * n4
                          + (unsigned __int128)3289354 * n2
                          - (unsigned __int128)20949480;

    unsigned __int128 numerator = n2 * poly;
    unsigned __int128 result128 = numerator / (unsigned __int128)720;
    return (seq_t)result128;
#else
    /* Fallback for compilers without __int128 support.
       Performs the computation without intermediate overflow by
       dividing by 720 progressively.  */
    seq_t res = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;           /* n^2     */
    seq_t n4 = n2 * n2;                       /* n^4     */
    seq_t n6 = n4 * n2;                       /* n^6     */
    seq_t n8 = n4 * n4;                       /* n^8     */
    seq_t n10 = n8 * n2;                      /* n^10    */

    /* Each term is (coeff * n^p) / 720 added to the sum. */
    /* 1 * n^12 */
    seq_t term = n2 * n10; /* n^12 */
    res += term / 720;

    /* -135 * n^10 */
    term = (seq_t)135 * n10;
    res += (seq_t)(-((long long)term / 720));

    /* +7525 * n^8 */
    term = (seq_t)7525 * n8;
    res += term / 720;

    /* -217665 * n^6 */
    term = (seq_t)217665 * n6;
    res -= term / 720;

    /* +3289354 * n^4 */
    term = (seq_t)3289354 * n4;
    res += term / 720;

    /* -20949480 * n^2 */
    term = (seq_t)20949480 * n2;
    res -= term / 720;

    return res;
#endif
}
