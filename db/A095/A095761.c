/*
 * A095761  a(n) = A014824(2*n-1).
 * https://oeis.org/A095761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = 100*a(n-1) + 22*n - 21 */
seq_t a(idx_t n)
{
    /* N < 1 is outside the offset; return 0 safely */
    if (n == 0)
        return 0;

    seq_t val = 1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* compute: val = 100*val + (22*i - 21) */
        val = val * (seq_t)100 + ((seq_t)i * 22 - 21);
    }
    return val;
}
