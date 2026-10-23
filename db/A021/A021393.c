/*
 * A021393  Decimal expansion of 1/389.
 * https://oeis.org/A021393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/389) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th digit of the decimal expansion of 1/389 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) mod 389 */
    uint64_t exp = n + 1;
    uint64_t mod389 = 389;
    uint64_t base = 10 % mod389;
    uint64_t r = 1;
    uint64_t e = exp;
    while (e)
    {
        if (e & 1ULL)
            r = (r * base) % mod389;
        base = (base * base) % mod389;
        e >>= 1ULL;
    }

    /* Compute 10^(n+1) mod 3890 */
    uint64_t mod3890 = 3890;
    base = 10 % mod3890;
    uint64_t M = 1;
    e = exp;
    while (e)
    {
        if (e & 1ULL)
            M = (M * base) % mod3890;
        base = (base * base) % mod3890;
        e >>= 1ULL;
    }

    /* Obtain digit: (M - r) / 389, guaranteed to be an integer 0–9 */
    seq_t digit = (seq_t)((M - r) / 389);
    return digit;
}
