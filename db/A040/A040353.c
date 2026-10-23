/*
 * A040353  Continued fraction for sqrt(373).
 * https://oeis.org/A040353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 19 if n == 0; otherwise a(n) = pattern[(n-1) mod 5] with
 *            pattern = {3,5,5,3,38}
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;

    /* Periodic block of the continued fraction of sqrt(373) */
    static const seq_t pattern[5] = { (seq_t)3, (seq_t)5, (seq_t)5, (seq_t)3, (seq_t)38 };

    /* Compute the index within the pattern */
    unsigned int idx = (unsigned int)((n - 1) % 5);
    return pattern[idx];
}
