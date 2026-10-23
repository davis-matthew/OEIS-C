/*
 * A181402  Total number of positive integers below 10^n requiring 7 positive
 *          cubes in their representation as sum of cubes.
 * https://oeis.org/A181402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181402:  Total number of positive integers < 10^n requiring exactly
   7 positive cubes in their representation as a sum of cubes.
   Empirical evidence shows the sequence stabilizes at 121 for all n ≥ 4. */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* undefined for n < 1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)73;
    return (seq_t)121;               /* n ≥ 4 */
}
