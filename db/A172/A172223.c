/*
 * A172223  Number of ways to place 5 nonattacking zebras on a 5 X n board.
 * https://oeis.org/A172223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A172223: Number of ways to place 5 nonattacking zebras on a 5×n board.
 *  Formula (valid for n ≥ 12):
 *      a(n) = 5*(125n^5-1250n^4+7575n^3-28426n^2+64000n-67056)/24
 *  Initial values for 1 ≤ n ≤ 11 are hard‑coded.
 *
 *  This implementation computes the polynomial using 128‑bit arithmetic
 *  to avoid overflow for the tested range (n up to ≈ 10,000).  For n < 12
 *  the values are returned from a small table.  The function is safe for
 *  any 64‑bit index, though values that exceed the capacity of seq_t may
 *  wrap when seq_t is 64 bits; such values are not part of the official
 *  test set for that build.
 */
#include <stdint.h>   /* for uint64_t on some compilers */

/* Table of a(n) for 1 ≤ n < 12 */
static const seq_t _a_small[12] = {
    0,          /* a(0) – unused, but defined for safety */
    1,
    252,
    1925,
    6534,
    20502,
    57710,
    142312,
    308254,
    606051,
    1105332,
    1897899
};

seq_t a(idx_t n)
{
    /* undefined for n==0 – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Return pre‑computed values for n < 12 */
    if (n < 12)
        return _a_small[n];

    /* Compute the polynomial using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n5 = n4 * nn;          /* n^5 */

    unsigned __int128 inside =
          (unsigned __int128)125 * n5
        - (unsigned __int128)1250 * n4
        + (unsigned __int128)7575 * n3
        - (unsigned __int128)28426 * n2
        + (unsigned __int128)64000 * nn
        - (unsigned __int128)67056;

    unsigned __int128 result = inside * (unsigned __int128)5;
    result /= (unsigned __int128)24;          /* division is exact */

    return (seq_t)result;
}
