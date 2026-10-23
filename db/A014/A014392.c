/*
 * A014392  Final 2 digits of 8^n.
 * https://oeis.org/A014392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8^n (mod 100)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the last two decimal digits of 8^n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus 100. Using 8^n mod 100. */
    seq_t result = 1;      /* 1 mod 100 */
    seq_t base   = 8;      /* 8 mod 100 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1ULL) {
            /* Multiply result and base modulo 100.
               Cast to an unsigned long long to avoid overflow
               for small 8‑bit types.  The product is ≤ 9801. */
            unsigned long long temp = (unsigned long long)result * (unsigned long long)base;
            result = (seq_t)(temp % 100);
        }

        /* Square base modulo 100. */
        unsigned long long temp = (unsigned long long)base * (unsigned long long)base;
        base = (seq_t)(temp % 100);

        exp >>= 1ULL;
    }

    return result;
}
