/*
 * A177682  a(n) = 6*a(n-1)-8*a(n-2)-3 for n > 4; a(0)=179, a(1)=1571,
 *          a(2)=12511, a(3)=155403, a(4)=572951.
 * https://oeis.org/A177682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 8*a(n-2) - 3 for n > 4,
   initial values: a(0)=179, a(1)=1571, a(2)=12511, a(3)=155403, a(4)=572951 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)179;
    if (n == 1u) return (seq_t)1571;
    if (n == 2u) return (seq_t)12511;
    if (n == 3u) return (seq_t)155403;
    if (n == 4u) return (seq_t)572951;

    seq_t prev2 = (seq_t)155403;   /* a(3) */
    seq_t prev1 = (seq_t)572951;   /* a(4) */
    for (idx_t k = 5; k <= n; ++k) {
        seq_t cur = prev1 * 6 - prev2 * 8 - 3;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
