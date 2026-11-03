/*
 * A083528  a(n) = 5^n mod 2*n.
 * https://oeis.org/A083528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n mod (2*n)
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 954 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Binary exponentiation: base^exp mod mod */
static seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    if (mod == 0)
        return 0;  /* undefined, but avoid division by zero */

    seq_t result = 1 % mod;
    base %= mod;

    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5;
    seq_t mod  = (seq_t)2 * (seq_t)n;
    return mod_pow_seq(base, (uint64_t)n, mod);
}
