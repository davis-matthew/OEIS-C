/*
 * A040223  Continued fraction for sqrt(239).
 * https://oeis.org/A040223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 15, then a(n) repeats the 12‑term block *
 *            (2,5,1,2,4,15,4,2,1,5,2,30) for n ≥ 1. * a(n) = period[(n-1) mod
 *            12] for n > 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period of the continued fraction of sqrt(239) after the initial term. */
    static const seq_t period[12] =
    {
        (seq_t)2,
        (seq_t)5,
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)15,
        (seq_t)4,
        (seq_t)2,
        (seq_t)1,
        (seq_t)5,
        (seq_t)2,
        (seq_t)30
    };

    if (n == 0)
        return (seq_t)15;

    /* Compute index in the periodic block. */
    idx_t idx = (n - 1) % (idx_t)12;
    return period[idx];
}
