/*
 * A073103  Number of solutions to x^4 == 1 (mod n).
 * https://oeis.org/A073103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} f(p,e) f(2,e) = 2^min(e-1,3) f(p,e) = 4 if p ≡
 *            1 (mod 4) f(p,e) = 2 if p ≡ 3 (mod 4) (the exponent e is
 *            relevant only for p = 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* handle factor 2 */
    idx_t e2 = 0;
    while ((temp & (idx_t)1) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2 > 0) {
        idx_t exp = e2 - 1;
        if (exp > 3) exp = 3;
        result *= ((seq_t)1 << exp);      /* 2^exp */
    }

    /* factor remaining odd part by trial division */
    idx_t p = 3;
    while (p <= temp / p) {
        if (temp % p == 0) {
            /* distinct odd prime found */
            if (p % 4 == 1) result *= (seq_t)4;
            else result *= (seq_t)2;
            /* remove all copies of this prime */
            do { temp /= p; } while (temp % p == 0);
        }
        p += 2;
    }
    /* any remaining prime > sqrt(original temp) */
    if (temp > 1) {
        if (temp % 4 == 1) result *= (seq_t)4;
        else result *= (seq_t)2;
    }

    return result;
}
