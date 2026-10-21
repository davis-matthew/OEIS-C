/*
 * A222259  Lexicographically earliest injective sequence of positive integers
 *          such that the sum of 8 consecutive terms is always divisible by 8.
 * https://oeis.org/A222259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 4 != 0) ? n : (2*n-4)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (offset is 1). */
    if (n == 0)
        return (seq_t)0;           /* undefined case; return 0 */

    if ((n & 3ULL) == 0ULL)        /* n divisible by 4 */
        return (seq_t)n * (seq_t)2 - (seq_t)4;
    else
        return (seq_t)n;
}
