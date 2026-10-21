/*
 * A082365  A Jacobsthal number sequence.
 * https://oeis.org/A082365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*8^n - (-1)^n)/3 Also satisfies the recurrence: a(0) =
 *            1, a(1) = 11, a(n) = 7*a(n-1) + 8*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)11;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)7 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
