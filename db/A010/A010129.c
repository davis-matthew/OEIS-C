/*
 * A010129  Continued fraction for sqrt(31).
 * https://oeis.org/A010129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9584, u16 n <= 9584, u32 n <= 9584, u64 n <= 9584,
 *            u128 n <= 9584, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction expansion of sqrt(31)
 *  a(0) = 5
 *  For n > 0, a(n) repeats the 8‑term block
 *  (1, 1, 3, 5, 3, 1, 1, 10).
 *  That is, a(n) = periodic[(n-1) mod 8] with
 *  periodic = {1,1,3,5,3,1,1,10}.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;

    /* The block length is a power of two (8 = 2³),
     * so we can use & 7 for modulo 8, which is
     * equivalent to (n-1) % 8 for unsigned integers. */
    const idx_t idx = (n - 1) & 7UL;

    static const seq_t periodic[8] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)3,
        (seq_t)5,
        (seq_t)3,
        (seq_t)1,
        (seq_t)1,
        (seq_t)10
    };

    return periodic[idx];
}
