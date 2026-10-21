/*
 * A237634  Number of (n+1)X(5+1) 0..2 arrays with the upper median of every
 *          2X2 subblock differing from its horizontal and vertical neighbors
 *          by exactly one.
 * https://oeis.org/A237634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * a(n-4) for n > 13 a(1) … a(13) are given below a(1) =
 *            4621, a(2) = 4500, a(3) = 2086, a(4) = 2816, a(5) = 4622, a(6) =
 *            5656, a(7) = 7756, a(8) = 11312, a(9) = 17626, a(10) = 22624,
 *            a(11) = 30496, a(12) = 45248, a(13) = 70792
 * Range:     u8 none, u16 n <= 12, u32 n <= 44, u64 n <= 108, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 64‑bit constants – safe for all builds of seq_t */
    static const unsigned long long init64[14] = {
        0ULL,          /* dummy element for 0-index */
        4621ULL, 4500ULL, 2086ULL, 2816ULL, 4622ULL, 5656ULL,
        7756ULL, 11312ULL, 17626ULL, 22624ULL, 30496ULL,
        45248ULL, 70792ULL
    };

    if (n <= 13ULL) {
        return (seq_t)init64[n];
    }

    /* Number of times the recurrence is applied.  For n>13,
       count = floor((n-10)/4)  ⇔  n - 4*count is reduced to the range 1..13. */
    idx_t count = (n - 10ULL) / 4ULL;          /* safe: n>13 guarantees n-10 >= 4 */
    idx_t m = n - 4ULL * count;               /* reduced index, 1 ≤ m ≤ 13 */

    seq_t result = (seq_t)init64[m];
    for (idx_t i = 0; i < count; ++i) {
        result *= (seq_t)4ULL;
    }
    return result;
}
