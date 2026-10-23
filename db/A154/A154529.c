/*
 * A154529  A090040 mod 9.
 * https://oeis.org/A154529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=1; for n>=3, a(n)=2^(n-2) mod 9
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)5;
    if (n == 2U) return (seq_t)1;

    /* For n>=3, exponent e = n-2 is at least 1.
       Since 2^k mod 9 repeats with period 6,
       compute e mod 6 and use a small lookup table. */
    idx_t e = (n - 2) % 6U;
    static const unsigned char modpow2[6] = { 1, 2, 4, 8, 7, 5 };
    return (seq_t)modpow2[e];
}
