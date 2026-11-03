/*
 * A071935  K(n,n+1) where K(x,y) is the Kronecker symbol (x/y).
 * https://oeis.org/A071935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = K(n, n+1) where K is the Kronecker symbol (·/·)
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Validated: partial, pass, 107 terms through n = 107 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Compute the Kronecker symbol (n | n+1) */

    /* Handle the trivial case n = 0 (should not occur) */
    if (n == 0)
        return 0;

    /* Separate the denominator into its 2-adic part and odd part */
    idx_t denom = n + 1;          /* n+1 is >0 */
    idx_t k = 0;                  /* exponent of 2 in denom */
    while ((denom & 1) == 0) {
        denom >>= 1;
        ++k;
    }

    /* Compute the Jacobi part for the odd part of the denominator */
    seq_t r = 1;                  /* default value if denom becomes 1 */
    if (denom != 1) {
        r = (seq_t)jacobi_symbol((uint64_t)n, (uint64_t)denom);
    }

    /* Compute (n/2)^k.
       Since n is odd when k>0, (n/2) is either +1 or -1. */
    uint8_t mod8 = (uint8_t)(n & 7ull);
    int base = (mod8 == 1 || mod8 == 7) ? 1 : -1;   /* (n/2) value */

    int sign = (k % 2 == 0) ? 1 : base;              /* (n/2)^k */

    return r * (seq_t)sign;
}
