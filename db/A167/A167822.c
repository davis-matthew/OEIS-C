/*
 * A167822  Subsequence of A167709 whose indices are congruent to 1 mod 5,
 *          i.e., a(n) = A167709(5*n+1).
 * https://oeis.org/A167822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=560; a(n+2)=340*a(n+1)-a(n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 560;

    seq_t prev2 = 1;
    seq_t prev1 = 560;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 tmp = (unsigned __int128)prev1 * 340
                                   - (unsigned __int128)prev2;
            next = (seq_t)tmp;
        } else {
            next = 340 * prev1 - prev2;
        }
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
