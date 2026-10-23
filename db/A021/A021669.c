/*
 * A021669  Decimal expansion of 1/665.
 * https://oeis.org/A021669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) / 665) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, the n-th term is the digit in the decimal expansion of 1/665
   with a(0) being the integer part. */

#include <stdint.h>

/* Compute 10^exp mod m, where exp is an idx_t and m fits into uint64_t */
static uint64_t pow10_mod(idx_t exp, uint64_t m)
{
    uint64_t result = 1 % m;
    uint64_t base   = 10 % m;

    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % m;
        base = (base * base) % m;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Moduli */
    const uint64_t mod1 = 6650ULL; /* 665 * 10, used to recover the last
                                     digit of the quotient 10^(n+1)/665 */
    const uint64_t mod2 = 665ULL;  /* 665, used to get the remainder */

    /* Compute 10^n mod mod1 and mod2, then multiply by 10 to account for
       exp = n + 1 without risking a wraparound of n+1. */
    uint64_t x = pow10_mod(n, mod1);
    uint64_t y = pow10_mod(n, mod2);

    x = (x * 10ULL) % mod1;  /* 10^(n+1) mod mod1 */
    y = (y * 10ULL) % mod2;  /* 10^(n+1) mod mod2 */

    /* Because mod1 = 10 * mod2, we have x ≡ y (mod mod2).  
       Therefore, (x - y) is a multiple of mod2 (= 665), and
       (x - y)/665 is the desired digit (0–9). */
    uint64_t digit = (x - y) / 665ULL;

    return (seq_t)digit;
}
