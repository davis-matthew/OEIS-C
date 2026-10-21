/*
 * A204688  a(n) = n^n (mod 3).
 * https://oeis.org/A204688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n (mod 3)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is a special case: 0^0 is defined as 1 */
    if (n == 0)
        return 1;

    /* Work modulo 3 since 3 is prime and small */
    seq_t base_mod = (seq_t)(n % 3);

    /* For base 0 → 0 for any positive exponent. */
    if (base_mod == 0)
        return 0;

    /* For base 1 → 1 for any exponent. */
    if (base_mod == 1)
        return 1;

    /* Base 2: powers cycle with period 2:
       2^odd ≡ 2  (mod 3)
       2^even ≡ 1 (mod 3) */
    return (n & (idx_t)1) ? (seq_t)2 : (seq_t)1;
}
