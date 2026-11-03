/*
 * A021958  Decimal expansion of 1/954.
 * https://oeis.org/A021958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/954) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (the n-th digit after the decimal point of 1/954) */

#include <stdint.h>

/* modular exponentiation of 10^exp modulo mod (mod <= 9540) */
static inline seq_t mod_exp10(idx_t exp, unsigned mod)
{
    unsigned result = 1 % mod;
    unsigned base   = 10 % mod;
    while (exp)
    {
        if (exp & 1U)
            result = (unsigned)((unsigned long long)result * base % mod);
        base = (unsigned)((unsigned long long)base * base % mod);
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* compute 10^(n+1) mod 954 and mod 9540 */
    idx_t e = n + 1;
    seq_t r = mod_exp10(e, 954);   /* 10^(n+1) mod 954   */
    seq_t s = mod_exp10(e, 9540);  /* 10^(n+1) mod 9540  */

    /* tmp = (s - r) modulo 9540, guaranteed to be a multiple of 954 */
    seq_t tmp;
    if (s >= r)
        tmp = s - r;
    else
        tmp = (seq_t)9540 - (r - s);

    return tmp / 954;              /* digit 0..9 */
}
