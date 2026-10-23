/*
 * A136702  Final nonzero digit of n! in base 16.
 * https://oeis.org/A136702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 16^{floor(v_2(n!)/4)}) mod 16
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The final non‑zero hexadecimal digit of n! is the product of the odd
 * factors of 1..n modulo 16 multiplied by 2^{(v_2(n!) mod 4)}.
 * This implementation uses a simple O(n) loop, suitable for n up to
 * at least 10000 in well under 2 s.  It works for any seq_t and idx_t
 * defined by the build. */

seq_t a(idx_t n)
{
    seq_t prod = 1;           /* product of odd factors modulo 16 */
    unsigned char e_mod4 = 0; /* v2(n!) modulo 4 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t x = k;
        unsigned int twos = 0;

        /* Strip all factors of 2 from x, count them */
        while ((x & 1) == 0) {
            x >>= 1;
            ++twos;
        }

        /* Update exponent of 2 modulo 4 */
        e_mod4 = (unsigned char)((e_mod4 + twos) & 3);

        /* Multiply the remaining odd part modulo 16 */
        prod = (prod * (seq_t)(x & 0xF)) & 0xFULL;
    }

    /* Multiply by 2^{e_mod4} modulo 16 */
    seq_t pow2 = (seq_t)1 << e_mod4;
    return (prod * pow2) & 0xFULL;
}
