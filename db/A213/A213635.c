/*
 * A213635  m*[n/m], where m is the least nondivisor of n (as in A007978) and
 *          [ ] = floor.
 * https://oeis.org/A213635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m * ⌊n/m⌋, where m is the least positive integer that
 *            does not divide n (the least nondivisor of n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the least nondivisor m. */
    idx_t m = 1;
    while (n % m == 0)      /* 1 always divides n for n>=1 */
        ++m;

    /* Compute floor(n/m) and multiply by m. */
    idx_t q = n / m;        /* integer division = floor(n/m) */
    return (seq_t)m * (seq_t)q;
}
