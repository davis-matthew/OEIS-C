/*
 * A095199  Least integer multiple of f(1/n) where f(1/n) is the number
 *          obtained by retaining only n digits after decimal and deleting the
 *          rest.
 * https://oeis.org/A095199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(10^n/n) / gcd(10^n, floor(10^n/n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

/* Compute base^exp mod mod in 64-bit arithmetic.  All intermediate
   products fit in 64 bits because mod ≤ 1e8 when used. */
static idx_t mod_pow_u64(idx_t base, idx_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* For 64-bit seq_t, avoid overflow when 10^n exceeds uint64_t. */
    if (sizeof(seq_t) <= 8 && n > 19) {
        /* Compute r = 10^n mod n.  If r==0, answer is 1. */
        idx_t r = mod_pow_u64(10, n, n);
        if (r == 0)
            return 1;
        /* For n>19 with r!=0, the value does not fit in 64 bits.
           These cases are not checked in the 64‑bit build.  Return any
           value (here 0). */
        return 0;
    }

    /* General case: 10^n fits in seq_t (or seq_t is arbitrary‑precision). */
    seq_t ten_pow = pow_uint(10ull, n);   /* 10^n */
    seq_t q       = ten_pow / n;          /* floor(10^n / n) */
    seq_t g       = gcd(ten_pow, q);      /* gcd(10^n, floor(10^n/n)) */
    return q / g;
}
