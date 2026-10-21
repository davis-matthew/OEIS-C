/*
 * A092431  Numbers having in binary representation a leading 1 followed by n
 *          zeros and n-1 ones.
 * https://oeis.org/A092431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n-1) + 2^(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; for n == 0 the function returns 0, but
       the sequence is defined for n >= 1. */
    if (n == 0)
        return 0;

    /* Compute the terms using left‑shift.  For fixed‑width builds
       n is guaranteed to be small enough that the shifts do not
       overflow; for arbitrary‑precision builds the shift is
       defined for any exponent. */
    seq_t term = ((seq_t)1 << (2*n-1)) + ((seq_t)1 << (n-1)) - 1;
    return term;
}
