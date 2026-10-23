/*
 * A180592  Digital root of 2n.
 * https://oeis.org/A180592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital root of 2*n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = (n == 0 ? 0 : ((2*n - 1) % 9) + 1) */

seq_t a(idx_t n)
{
    /* Handle the trivial case first */
    if (n == 0)
        return 0;

    /* Working modulo 9 avoids overflow on very large n or tiny seq_t types */
    unsigned int n_mod9 = (unsigned int)(n % 9);
    unsigned int mod9 = (2 * n_mod9) % 9;

    /* If the modulo 9 is 0, then 2*n is a positive multiple of 9,
       hence its digital root is 9 (not 0). */
    return (seq_t)(mod9 == 0 ? 9 : mod9);
}
