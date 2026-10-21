/*
 * A237635  Number of (n+1)X(6+1) 0..2 arrays with the upper median of every
 *          2X2 subblock differing from its horizontal and vertical neighbors
 *          by exactly one.
 * https://oeis.org/A237635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-4) for n>14, with a(1)=11936, a(2)=8594, a(3)=2900,
 *            a(4)=4440, a(5)=5656, a(6)=9368, a(7)=11984, a(8)=20756,
 *            a(9)=28336, a(10)=47134, a(11)=59920, a(12)=104172,
 *            a(13)=141680, a(14)=235572, a(15)=299600, a(16)=520860,
 *            a(17)=708400, a(18)=1177860
 * Range:     u8 none, u16 n <= 11, u32 n <= 38, u64 n <= 93, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static table of the first 18 terms.  Index 0 is unused. */
    static const seq_t init[19] = {
        0ULL,
        11936ULL, 8594ULL, 2900ULL, 4440ULL, 5656ULL,
        9368ULL, 11984ULL, 20756ULL, 28336ULL, 47134ULL,
        59920ULL, 104172ULL, 141680ULL, 235572ULL,
        299600ULL, 520860ULL, 708400ULL, 1177860ULL
    };

    if (n <= 18)
        return init[n];

    /* For n > 18 use the recurrence a(n)=5*a(n-4).  We
       reduce the index modulo 4 to a base index between 15 and 18,
       then multiply by 5^k where k = (n-15)/4.                     */
    idx_t base = 15 + ((n - 15) & 3ULL);   /* n%4 cycle: 15,16,17,18 */
    uint64_t k = (n - 15) / 4;
    seq_t res = init[base];
    for (uint64_t i = 0; i < k; ++i)
        res *= 5;
    return res;
}
