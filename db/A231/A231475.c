/*
 * A231475  Largest integer less than 5 and coprime to n.
 * https://oeis.org/A231475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n is odd; otherwise a(n) = 1 if (n/2) divisible by
 *            3, else 3
 * Range:     u8 n <= 9885, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1U) {            /* n is odd */
        return (seq_t)4;
    } else {                 /* n is even */
        idx_t k = n >> 1;    /* k = n/2 */
        return (seq_t)((k % 3U) == 0U ? 1 : 3);
    }
}
