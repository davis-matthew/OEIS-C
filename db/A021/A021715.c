/*
 * A021715  Decimal expansion of 1/711.
 * https://oeis.org/A021715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/711) mod 10 (decimal digit at position n
 *            of 1/711)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 7110.  Since 7110 = 711*10,
       the quotient floor(10^(n+1)/711) modulo 10 equals
       (10^(n+1) mod 7110) / 711 because the remainder after
       dividing by 7110 is at most 7109 < 7110.  The division by
       711 yields a single decimal digit (0‑9). */
    seq_t powmod = mod_pow((seq_t)10, n + 1, (seq_t)7110);
    return powmod / 711;
}
