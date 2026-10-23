/*
 * A035144  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -46.
 * https://oeis.org/A035144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum_{d|n} Kronecker(-46,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative formula:
   a(p^e) = 1           if Kronecker(-46,p) = 0   (p = 2 or 23)
   a(p^e) = (1+(-1)^e)/2 if Kronecker(-46,p) = -1
   a(p^e) = e+1         if Kronecker(-46,p) = 1
   which is equivalent to the divisor‑sum definition above.  */
#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n >= 1 in practice */
    uint64_t temp = (uint64_t)n;
    seq_t res = 1;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0) continue;
        unsigned e = 0;
        while (temp % p == 0)       /* count exponent of prime p */
        {
            temp /= p;
            ++e;
        }
        seq_t part;
        int symbol;
        if (p == 2 || p == 23) {
            symbol = 0;             /* Kronecker(-46,p) = 0 for p = 2 or 23 */
        } else {
            uint64_t a_mod = (p - (46ULL % p)) % p;  /* (-46 mod p) */
            symbol = jacobi_symbol(a_mod, p);        /* ±1 or 0 */
        }
        if (symbol == 0) {                  /* a(p^e) = 1 */
            part = 1;
        } else if (symbol == -1) {          /* a(p^e) = (1+(-1)^e)/2 */
            part = ((e & 1) == 0) ? 1 : 0;
        } else {                            /* symbol == 1 */
            part = (seq_t)(e + 1);
        }
        res *= part;
        if (res == 0) return 0;             /* early exit */
    }
    /* Remaining prime factor if any */
    if (temp > 1) {
        uint64_t p = temp;
        unsigned e = 1;
        seq_t part;
        int symbol;
        if (p == 2 || p == 23) {
            symbol = 0;
        } else {
            uint64_t a_mod = (p - (46ULL % p)) % p;
            symbol = jacobi_symbol(a_mod, p);
        }
        if (symbol == 0) {
            part = 1;
        } else if (symbol == -1) {
            part = ((e & 1) == 0) ? 1 : 0;
        } else {
            part = (seq_t)(e + 1);
        }
        res *= part;
    }
    return res;
}
