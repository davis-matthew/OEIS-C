/*
 * A221462  Number of 0..7 arrays of length n with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221462:  Number of 0..7 arrays of length n with each element unequal to at least one neighbor, starting with 0.
   Recurrence: a(1) = 0;  a(2) = 7;  a(n) = 7*a(n-1) + 7*a(n-2)  for n >= 3. */
seq_t a(idx_t n)
{
    /* n >= 1 by definition; treat n==0 as 0 for safety */
    if (n <= 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;

    seq_t prev2 = (seq_t)0; /* a(1) */
    seq_t prev1 = (seq_t)7; /* a(2) */
    seq_t cur    = 0;

    /* iterate the linear recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)7 * prev1 + (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
