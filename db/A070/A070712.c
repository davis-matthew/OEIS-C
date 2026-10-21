/*
 * A070712  a(n) = n^7 mod 31.
 * https://oeis.org/A070712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 31
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 31, then raise to 7 modulo 31.
       All intermediate values stay < 31, so a simple uint32_t is safe. */
    uint32_t r   = (uint32_t)(n % 31u);
    uint32_t res = 1u;
    for (int i = 0; i < 7; ++i)
        res = (res * r) % 31u;
    return (seq_t)res;
}
