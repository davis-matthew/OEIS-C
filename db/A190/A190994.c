/*
 * A190994  a(n) = a(n-1) + a(n-2), for n>=2, with a(0)=27, a(1)=2.
 * https://oeis.org/A190994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-1) + a(n-2)   for n >= 2
   a(0) = 27
   a(1) = 2
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)27; /* a(n-2) */
    seq_t prev1 = (seq_t)2;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
