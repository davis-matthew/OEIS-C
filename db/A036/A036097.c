/*
 * A036097  Centered cube numbers: (n+1)^19+n^19.
 * https://oeis.org/A036097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^19 + n^19
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 9, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the output type for safe arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Base values for the two terms */
    seq_t base1 = n_seq + (seq_t)1;   /* n + 1 */
    seq_t base2 = n_seq;              /* n          */

    /* Compute base^19 by repeated multiplication (19 multiplications) */
    seq_t pow1 = base1;
    seq_t pow2 = base2;

    /* 19 = 1 + 18, so we multiply 18 more times */
    for (idx_t i = 1; i < 19; ++i) {
        pow1 *= base1;
        pow2 *= base2;
    }

    /* Summation gives the centered cube number */
    return pow1 + pow2;
}
