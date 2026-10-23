/*
 * A014637  Odd heptagonal numbers (A000566).
 * https://oeis.org/A014637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=55, a(3)=81,
 * a(n) = 80 + 2*a(n-2) - a(n-4)  for n ≥ 4   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)55;
    if (n == 3) return (seq_t)81;

    if ((n & 1) == 0) {               /* even n */
        seq_t prev2 = (seq_t)55;      /* a(n-2)   */
        seq_t prev0 = (seq_t)1;       /* a(n-4)   */
        seq_t curr;
        for (idx_t i = 4; i <= n; i += 2) {
            curr = (seq_t)80 + (seq_t)2 * prev2 - prev0;
            prev0 = prev2;
            prev2 = curr;
        }
        return prev2;
    } else {                          /* odd n */
        seq_t prev2 = (seq_t)81;      /* a(n-2)   */
        seq_t prev0 = (seq_t)7;       /* a(n-4)   */
        seq_t curr;
        for (idx_t i = 5; i <= n; i += 2) {
            curr = (seq_t)80 + (seq_t)2 * prev2 - prev0;
            prev0 = prev2;
            prev2 = curr;
        }
        return prev2;
    }
}
