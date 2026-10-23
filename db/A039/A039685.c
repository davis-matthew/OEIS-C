/*
 * A039685  Numbers m such that m^2 ends in 444.
 * https://oeis.org/A039685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039685 – Numbers m such that m^2 ends in 444.
   Formula: a(n) = 250*n - 212 for odd n;  a(n) = 250*n - 38 for even n. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n==0 we return 0
       (offset is 1). */
    if (n == 0U) return 0;
    if ((n & 1) == 0) {
        /* n even -> a(n) = 250*n - 38 */
        return 250 * (seq_t)n - (seq_t)38;
    } else {
        /* n odd  -> a(n) = 250*n - 212 */
        return 250 * (seq_t)n - (seq_t)212;
    }
}
