/*
 * A047261  Numbers that are congruent to {2, 4, 5} mod 6.
 * https://oeis.org/A047261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers ≡ {2,4,5} (mod 6) a(3k+1) = 6k + 2 a(3k+2) = 6k +
 *            4 a(3k+3) = 6k + 5
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of bounds */
    seq_t nn = (seq_t)n;                  /* cast index to seq_t */
    seq_t k  = (nn - 1) / 3;              /* block number */
    seq_t r  = (nn - 1) % 3;              /* position inside block */

    if (r == 0) return 6 * k + 2;
    if (r == 1) return 6 * k + 4;
    return 6 * k + 5;                    /* r == 2 */
}
