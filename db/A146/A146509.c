/*
 * A146509  Numbers that are congruent to {1, 5} mod 18.
 * https://oeis.org/A146509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = 18*(k-1)+1, a(2k) = 18*(k-1)+5
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* offset irrelevant */
    }
    if ((n & 1) != 0) {           /* odd index: a(2k-1) */
        seq_t half = (seq_t)((n - 1) / 2);
        return (seq_t)18 * half + (seq_t)1;
    } else {                      /* even index: a(2k)   */
        seq_t half = (seq_t)(n / 2);
        return (seq_t)18 * half - (seq_t)13;
    }
}
