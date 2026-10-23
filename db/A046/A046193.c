/*
 * A046193  Indices of heptagonal numbers (A000566) which are also triangular
 *          numbers (A000217).
 * https://oeis.org/A046193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 322*a(n-2) - a(n-4) - 96 (n ≥ 5) Initial values: a(1)=1,
 *            a(2)=5, a(3)=221, a(4)=1513 This recurrence generates the
 *            indices of heptagonal numbers that are also triangular
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for n<1 is arbitrary */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)221;
    if (n == 4) return (seq_t)1513;

    /* buffer holds the four most recent terms a(k-4)…a(k-1) */
    seq_t buf[4] = {(seq_t)1, (seq_t)5, (seq_t)221, (seq_t)1513};

    seq_t new_val;
    for (idx_t k = 5; k <= n; ++k) {
        /* indices for a(k-2) and a(k-4) in the circular buffer */
        idx_t idx_n2 = (k - 3) % 4;   /* (k-2-1) % 4 */
        idx_t idx_n4 = (k - 5) % 4;   /* (k-4-1) % 4 */

        new_val = (seq_t)322 * buf[idx_n2] - buf[idx_n4] - (seq_t)96;
        buf[(k - 1) % 4] = new_val;  /* store a(k) in the buffer */
    }

    return buf[(n - 1) % 4];
}
