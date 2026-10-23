/*
 * A191467  a(n) = 9^n - 7^n.
 * https://oeis.org/A191467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed with simple repeated multiplication; works for any unsigned
   seq_t type (8, 16, 32, 64, 128 bits or an arbitrary‑precision integer class). */
seq_t a(idx_t n)
{
    /* Base constants cast to seq_t. */
    const seq_t nine   = (seq_t)9;
    const seq_t seven  = (seq_t)7;

    /* Compute 9^n and 7^n by repeated multiplication. */
    seq_t pow9 = (seq_t)1;
    seq_t pow7 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow9 *= nine;
        pow7 *= seven;
    }

    /* Return the difference.  For all n >= 0 we have 9^n > 7^n. */
    return pow9 - pow7;
}
