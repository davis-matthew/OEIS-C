/*
 * A040560  Continued fraction for sqrt(585).
 * https://oeis.org/A040560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 24; for n > 0, a(n) = period[(n-1) mod 8]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where period = {5, 2, 1, 4, 1, 2, 5, 48} is the repeating block
   of the continued‑fraction expansion of sqrt(585). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)24;
    }
    static const seq_t period[8] =
        { (seq_t)5, (seq_t)2, (seq_t)1, (seq_t)4,
          (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)48 };
    idx_t k = (n - 1) % 8;
    return period[k];
}
