/*
 * A047371  Numbers that are congruent to {0, 2, 3, 5} mod 7.
 * https://oeis.org/A047371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n - 6) / 4)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer that is congruent to 0, 2, 3 or 5 modulo 7.
   The sequence starts at a(1) = 0. */
seq_t a(idx_t n)
{
    /* Use a wider type for the intermediate calculation to avoid
       overflow on small seq_t types (e.g. uint8_t). */
    unsigned long long tmp = 7ULL * (unsigned long long)n - 6ULL;
    seq_t result = (seq_t)(tmp / 4ULL);
    return result;
}
