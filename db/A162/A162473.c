/*
 * A162473  Write n in binary n times and concatenate (see example). a(n) is
 *          the decimal equivalent.
 * https://oeis.org/A162473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n * (2^(b*n) - 1) / (2^b - 1), where b = ⌊log₂(n)⌋ + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to writing the binary representation of n, n times, then interpreting as a binary number. */
seq_t a(idx_t n)
{
    /* Compute number of bits of n (b). */
    idx_t b = 0;
    idx_t t = n;
    while (t > 0) {
        t >>= 1;
        ++b;
    }
    /* Concatenate n’s binary representation n times. */
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = (val << b) | (seq_t)n;
    }
    return val;
}
