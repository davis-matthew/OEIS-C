/*
 * A228217  Number of second differences of arrays of length n+2 of numbers in
 *          0..7.
 * https://oeis.org/A228217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) for n > 15 Initial values: a(1)=29,
 *            a(2)=519, a(3)=7309, a(4)=88215, a(5)=951049, a(6)=9399615,
 *            a(7)=86929081, a(8)=766106895, a(9)=6537612649,
 *            a(10)=54701587935, a(11)=452558355481, a(12)=3719467815855,
 *            a(13)=30436607366089, a(14)=248242046141055,
 *            a(15)=2019169299698041
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 1..15 */
    static const seq_t base[15] = {
        (seq_t)29,
        (seq_t)519,
        (seq_t)7309,
        (seq_t)88215,
        (seq_t)951049,
        (seq_t)9399615,
        (seq_t)86929081,
        (seq_t)766106895,
        (seq_t)6537612649ULL,
        (seq_t)54701587935ULL,
        (seq_t)452558355481ULL,
        (seq_t)3719467815855ULL,
        (seq_t)30436607366089ULL,
        (seq_t)248242046141055ULL,
        (seq_t)2019169299698041ULL
    };

    if (n == 0) {
        return (seq_t)0;            /* offset is 1; value for n=0 not defined */
    }
    if (n <= 15) {
        return base[n - 1];
    }

    /* a(14) and a(15) are initial values for recurrence */
    seq_t prev2 = base[13];   /* a(14) */
    seq_t prev1 = base[14];   /* a(15) */
    seq_t cur;

    for (idx_t k = 16; k <= n; ++k) {
        cur = prev1 * (seq_t)15 - prev2 * (seq_t)56;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
