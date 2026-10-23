/*
 * A167823  Subsequence of A167709 whose indices are congruent to 2 mod 5,
 *          i.e., a(n) = A167709(5*n+2).
 * https://oeis.org/A167823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=15, a(1)=5124, a(n+2)=340*a(n+1)-a(n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial terms explicitly */
    if (n == 0)
        return (seq_t)15;
    if (n == 1)
        return (seq_t)5124;

    /* Recurrence: a(n+2) = 340*a(n+1) - a(n) */
    seq_t prev = (seq_t)15;      /* a(0) */
    seq_t curr = (seq_t)5124;    /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)340 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;  /* a(n) */
}
