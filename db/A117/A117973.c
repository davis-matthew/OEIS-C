/*
 * A117973  a(n) = 2^(wt(n)+1), where wt() = A000120().
 * https://oeis.org/A117973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(wt(n)+1) where wt(n) is the binary weight of n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the number of 1-bits in n */
    unsigned int bits = __builtin_popcountll((unsigned long long)n);

    /* Compute 2^(bits+1) safely without left‑shift overflow */
    seq_t result = 1;
    for (unsigned int i = 0; i < bits + 1; ++i)
        result *= 2;
    return result;
}
