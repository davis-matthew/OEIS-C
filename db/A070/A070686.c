/*
 * A070686  n^6 mod 46.
 * https://oeis.org/A070686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 46
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work modulo 46. */
    static const seq_t MOD = 46;
    seq_t r = ((seq_t)n) % MOD;   /* Reduce first: keeps intermediate small. */
    seq_t pow = r;                /* r^1 */

    /* Compute r^6 iteratively, reducing modulo at each step. */
    /* Use a small loop: 6 multiplications. */
    for (int i = 1; i < 6; ++i)
        pow = (pow * r) % MOD;

    return pow;
}
