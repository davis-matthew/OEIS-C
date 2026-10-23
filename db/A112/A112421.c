/*
 * A112421  Number of 6 element subsets of {1,2,3,...,n} for which the sum-set
 *          has 12 elements.
 * https://oeis.org/A112421
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/6)*(n-3*floor(n/6)-3) for n >= 7 (equivalently
 *            a(n) = 2*A008724(n-3) as per OEIS entry)
 * Range:     u8 n <= 42, u16 n <= 630, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 7, behaviour is unspecified; return 0. */
    if (n < 7) {
        return (seq_t)0;
    }

    /* j = floor(n/6) */
    idx_t j = n / 6;

    /* Convert everything to seq_t for arithmetic with the appropriate type. */
    seq_t jseq = (seq_t)j;
    seq_t nseq = (seq_t)n;
    seq_t term = nseq - (seq_t)3 * jseq - (seq_t)3;

    return ((seq_t)2) * jseq * term;
}
