/*
 * A047228  Numbers that are congruent to {2, 3, 4} mod 6.
 * https://oeis.org/A047228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047228: numbers congruent to {2,3,4} mod 6 (sorted).
   Formula: a(n) = 6*((n-1)/3) + ((n-1)%3) + 2,   n >= 1
   This implementation uses only integer arithmetic and works for any
   seq_t width that can represent the result. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 0; return 0 as safe placeholder */
        return (seq_t)0;
    idx_t m = n - 1;          /* shift to 0‑based index */
    idx_t q = m / 3;          /* number of full 6‑block cycles */
    idx_t r = m % 3;          /* position inside the current block */
    return ((seq_t)q * 6) + (seq_t)r + 2;
}
