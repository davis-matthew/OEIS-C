/*
 * A166931  Numbers m such that m mod k is k-1 for all k = 2..9.
 * https://oeis.org/A166931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2520*n - 1
 * Range:     u8 none, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2520*n - 1.  The multiplication and subtraction are
       performed in the target integer type seq_t to keep results
       exact whenever they fit. */
    seq_t res = (seq_t)n * (seq_t)2520 - (seq_t)1;
    return res;
}
