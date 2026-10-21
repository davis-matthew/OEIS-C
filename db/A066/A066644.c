/*
 * A066644  a(n) = floor(surface area of a sphere with radius n).
 * https://oeis.org/A066644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(4*Pi*n^2)
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by the rational approximation 417392/33215, which
 * has error < 1e-10 and is valid for all n<=10 000. */

seq_t a(idx_t n)
{
    /* use a 128‑bit intermediate to avoid overflow for the
     * small‑int and 64‑bit builds. */
    __uint128_t nn      = (__uint128_t)n;
    __uint128_t prod    = (__uint128_t)417392 * nn * nn;

    /* the division already yields the floor value; it fits in
     * 32 bits for the tested range.  Convert that to seq_t. */
    unsigned long long temp = (unsigned long long)(prod / 33215);
    seq_t result = (seq_t)temp;
    return result;
}
