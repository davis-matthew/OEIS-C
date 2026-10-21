/*
 * A070407  a(n) = 7^n mod 17.
 * https://oeis.org/A070407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 17
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // for bool, though not used here

seq_t a(idx_t n)
{
    /* Modulus for the sequence */
    static const seq_t MOD = (seq_t)17;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)7;

    /* Binary exponentiation: run over bits of n */
    while (n > 0)
    {
        if (n & ((idx_t)1))
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        n >>= 1;
    }

    return result;
}
