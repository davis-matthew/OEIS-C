/*
 * A155628  a(n) = 7^n-4^n+1^n.
 * https://oeis.org/A155628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 4^n + 1^n Recurrence (valid for all n ≥ 0): a(0) =
 *            1 a(1) = 4 a(n) = 11*a(n-1) - 28*a(n-2) + 18 (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)4;      /* a(1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t next = (seq_t)11 * prev1 - (seq_t)28 * prev2 + (seq_t)18;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
