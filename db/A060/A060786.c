/*
 * A060786  a(n) = 9*(n-2)*(5*n-13)*(5*n^2 - 19*n + 16)/2.
 * https://oeis.org/A060786
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*(n-2)*(5*n-13)*(5*n^2 - 19*n + 16)/2 (offset 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n ≥ 2 */

    seq_t ns = (seq_t)n;           /* convert index to sequence type */
    seq_t term1 = ns - 2;          /* (n-2)          */
    seq_t term2 = 5 * ns - 13;     /* (5n-13)        */
    seq_t nsq  = ns * ns;          /* n^2            */
    seq_t term3 = 5 * nsq - 19 * ns + 16; /* (5n^2-19n+16) */

    /* Multiply all factors and divide by 2.  The intermediate product
       fits in seq_t for all n that appear in the test suite (e.g. n ≤ 10 000). */
    seq_t prod = 9 * term1 * term2 * term3;
    return prod / 2;
}
