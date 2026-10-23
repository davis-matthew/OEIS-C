/*
 * A167775  Subsequence of A167708 whose indices are congruent to 1 mod 5,
 *          i.e., a(n) = A167708(5n+1).
 * https://oeis.org/A167775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2)=340*a(n+1)-a(n); a(0)=10, a(1)=2441
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)2441;

    seq_t prevprev = (seq_t)10;   /* a(0) */
    seq_t prev     = (seq_t)2441; /* a(1) */
    seq_t cur;

    /* iterate the linear recurrence a(n+2)=340*a(n+1)-a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        cur      = (seq_t)340 * prev - prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return prev; /* prev holds a(n) after the loop */
}
