/*
 * A054967  Numbers that are congruent to {0, 1, 9} mod 10.
 * https://oeis.org/A054967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k-2) = 10k-10, a(3k-1) = 10k-9, a(3k) = 10k-1
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the caller */
    idx_t r = n % 3U;          /* remainder of n modulo 3 */
    seq_t k, result;

    if (r == 1) {              /* n = 3k-2  →  10k-10 */
        k = ((seq_t)n + 2) / 3;
        result = k * 10 - 10;
    } else if (r == 2) {       /* n = 3k-1  →  10k-9  */
        k = ((seq_t)n + 1) / 3;
        result = k * 10 - 9;
    } else {                   /* r == 0 : n = 3k  →  10k-1  */
        k = (seq_t)n / 3;
        result = k * 10 - 1;
    }

    return result;
}
