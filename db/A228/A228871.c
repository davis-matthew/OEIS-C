/*
 * A228871  Odd numbers producing 3 out-of-order odd numbers in the Collatz
 *          (3x+1) iteration.
 * https://oeis.org/A228871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64^n/2 - 5)/9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3, a(2) = 227, a(n) = 65*a(n-1) - 64*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)227;

    seq_t prev2 = (seq_t)3;        /* a(1) */
    seq_t prev1 = (seq_t)227;      /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = ((seq_t)65) * prev1 - ((seq_t)64) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
