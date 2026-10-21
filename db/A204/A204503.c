/*
 * A204503  Squares n^2 such that floor(n^2/9) is again a square.
 * https://oeis.org/A204503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n-1)^2 for 1 <= n <= 5; a(n) = 9*(n-4)^2 for n > 5
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small exceptional cases directly. */
    if (n <= 5) {
        seq_t m = (seq_t)(n - 1);
        return m * m;
    }

    /* For all larger indices the sequence follows the quadratic form. */
    seq_t m = (seq_t)(n - 4);
    seq_t sq = m * m;
    return ((seq_t)9) * sq;
}
