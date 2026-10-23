/*
 * A024314  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (natural numbers >= 3), t = A023532.
 * https://oeis.org/A024314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 6*F(2*n+3)+F(2*n+1)-(n+6)*F(n+3)-(n+1)*F(n+1) a(2*n+1)
 *            = 6*F(2*n+2)+F(2*n)-(n+6)*F(n+2)-(n+1)*F(n) where F is the
 *            Fibonacci sequence
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A024314 using the Fibonacci formulas above. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if ((n & 1ULL) == 0) {                    /* even n : n = 2*k */
        idx_t k   = n >> 1;
        seq_t F2k3 = fib(2*k + 3);
        seq_t F2k1 = fib(2*k + 1);
        seq_t Fk3  = fib(k + 3);
        seq_t Fk1  = fib(k + 1);

        seq_t term1 = (seq_t)6 * F2k3;
        seq_t term2 = F2k1;
        seq_t term3 = (seq_t)(k + 6) * Fk3;
        seq_t term4 = (seq_t)(k + 1) * Fk1;

        return term1 + term2 - term3 - term4;
    } else {                                  /* odd n : n = 2*k-1 , k=(n+1)/2 */
        idx_t k   = (n + 1) >> 1;
        seq_t F2k2 = fib(2*k + 2);
        seq_t F2k   = fib(2*k);
        seq_t Fk2   = fib(k + 2);
        seq_t Fk    = fib(k);

        seq_t term1 = (seq_t)6 * F2k2;
        seq_t term2 = F2k;
        seq_t term3 = (seq_t)(k + 6) * Fk2;
        seq_t term4 = (seq_t)(k + 1) * Fk;

        return term1 + term2 - term3 - term4;
    }
}
