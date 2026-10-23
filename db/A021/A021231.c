/*
 * A021231  Decimal expansion of 1/227.
 * https://oeis.org/A021231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/227) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 227;          /* modulus = denominator 227            */
    seq_t base = 10 % MOD;          /* 10 modulo MOD                        */
    seq_t result = 1;               /* 10^0 mod MOD                         */
    idx_t exp = n;                  /* exponent n (since we need 10^n mod M)*/

    /* modular exponentiation: compute 10^n (mod 227) */
    while (exp > 0) {
        if (exp & 1U) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }

    seq_t r = result;               /* r = 10^n mod 227                     */
    seq_t digit = (r * 10) / MOD;    /* floor((r*10)/227) == desired digit  */
    return digit;
}
