/*
 * A208377  Number of n X 6 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208377
   Formula: a(n) = 364*n^2 - 416*n + 52  (for n > 1)
   Initial term: a(1) = 26                                                   */
seq_t a(idx_t n)
{
    if (n == 1) return 26;
    unsigned long long nn = n;
    unsigned long long val = 364ULL * nn * nn - 416ULL * nn + 52ULL;
    return (seq_t)val;
}
