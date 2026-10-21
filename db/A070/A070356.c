/*
 * A070356  a(n) = 3^n mod 23.
 * https://oeis.org/A070356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 3 is coprime to 23, and 3^11 ≡ 1 (mod 23), so the sequence is periodic
     * with period 11.  Compute 3^e mod 23 where e = n mod 11.  The exponent
     * e is at most 10, so a simple loop is sufficient.  The result always
     * stays below 23, fitting into any seq_t type. */
    idx_t e = n % 11;
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result = (result * 3) % 23;
    return result;
}
