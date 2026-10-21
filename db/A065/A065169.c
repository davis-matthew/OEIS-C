/*
 * A065169  Permutation t->t-2 of Z, folded to N.
 * https://oeis.org/A065169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+4 if n is odd, * n-4 if n is even and n>4, * 3 if n =
 *            2, * 1 if n = 4 * (a(1)=5, a(2)=3, a(3)=7, a(4)=1, …)
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n≥1 we never hit below the offset. */
    if (n & 1ULL)              /* odd n */
        return (seq_t)n + 4;
    else {                     /* even n */
        if (n > 4)
            return (seq_t)n - 4;
        else if (n == 2)
            return 3;
        else                    /* n == 4 */
            return 1;
    }
}
