/*
 * A010416  Squares mod 55.
 * https://oeis.org/A010416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth smallest integer r such that r ≡ k² (mod 55) for
 *            some integer k
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n > 18)               /* the sequence is finite: only 18 residues */
        return (seq_t)0;

    unsigned char seen[55] = {0};
    for (idx_t k = 0; k < 55; ++k) {
        seq_t r = (seq_t)((seq_t)k * k % (seq_t)55);
        seen[(unsigned int)r] = 1;
    }

    idx_t count = 0;
    for (unsigned int val = 0; val < 55; ++val) {
        if (seen[val]) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
    return (seq_t)0;   /* fallback – should not be reached */
}
