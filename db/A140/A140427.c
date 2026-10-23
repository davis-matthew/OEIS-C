/*
 * A140427  Arises in relating doubly-even error-correcting codes, graphs and
 *          irreducible representations of N-extended supersymmetry.
 * https://oeis.org/A140427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140427: a(n) = 0 for n < 4,
   a(n) = a(n-8) + 4 for n > 7,
   with base values: a(0..7) = {0,0,0,0,1,1,2,3}. */
seq_t a(idx_t n)
{
    /* Base values for indices 0 through 7. */
    static const seq_t base[8] = {0, 0, 0, 0, 1, 1, 2, 3};

    /* Compute n mod 8 and floor(n/8). */
    idx_t r = n & 7;           /* remainder modulo 8 */
    idx_t t = n >> 3;          /* integer division by 8 */

    /* a(n) = base[r] + 4 * t. */
    return base[r] + (seq_t)t * 4;
}
