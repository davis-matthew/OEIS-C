/*
 * A040917  Continued fraction for sqrt(948).
 * https://oeis.org/A040917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(2^e)=3, a(5^e)=20, a(p^e)=1 for odd p≠5; a(0)=30 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    /* has2 = 1 iff n is even, has5 = 1 iff n divisible by 5 */
    idx_t has2 = (n & 1) ^ 1;           /* 1 if even, 0 if odd */
    idx_t has5 = (n % 5) == 0;          /* 1 if n%5==0 */
    if (has2) {
        if (has5)
            return (seq_t)60;
        return (seq_t)3;
    }
    if (has5)
        return (seq_t)20;
    return (seq_t)1;
}
