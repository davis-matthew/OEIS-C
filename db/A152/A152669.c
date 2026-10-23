/*
 * A152669  Last digit of Catalan number A000108(n).
 * https://oeis.org/A152669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Catalan(n) mod 10. Catalan(n) = binom(2n,n)/(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Inverse modulo 10 for the residues 1,3,7,9 (all other entries are unused). */
static const seq_t inv_mod10[10] = {0, 1, 0, 7, 0, 0, 0, 3, 0, 9};

seq_t a(idx_t n)
{
    seq_t res = 1;          /* product of all odd factors after cancelling 2 and 5 */
    idx_t e2 = 0;           /* net exponent of 2 in the product */
    idx_t e5 = 0;           /* net exponent of 5 in the product */
    idx_t k;

    /* Compute binom(2n,n) by multiplying (n+k)/k for k=1..n,
       canceling factors 2 and 5 as they appear. */
    for (k = 1; k <= n; ++k) {
        idx_t x = n + k;                       /* numerator */
        while ((x & 1) == 0) {                 /* factor 2 */
            ++e2;
            x >>= 1;
        }
        while (x % 5 == 0) {                   /* factor 5 */
            ++e5;
            x /= 5;
        }
        res = (res * (x % 10)) % 10;

        idx_t y = k;                           /* denominator */
        while ((y & 1) == 0) {                 /* factor 2 */
            --e2;
            y >>= 1;
        }
        while (y % 5 == 0) {                   /* factor 5 */
            --e5;
            y /= 5;
        }
        /* y is now coprime to 10; divide by it via its modular inverse */
        res = (res * inv_mod10[y % 10]) % 10;
    }

    /* Divide by the remaining factor (n+1). */
    idx_t x = n + 1;
    while ((x & 1) == 0) {
        --e2;
        x >>= 1;
    }
    while (x % 5 == 0) {
        --e5;
        x /= 5;
    }
    res = (res * inv_mod10[x % 10]) % 10;

    /* Multiply back the remaining powers of 2 and 5. */
    if (e2 > 0) {
        seq_t pow2;
        switch (e2 % 4) {
            case 1: pow2 = 2; break;
            case 2: pow2 = 4; break;
            case 3: pow2 = 8; break;
            default: pow2 = 6; /* e2 % 4 == 0 with e2 > 0 */
        }
        res = (res * pow2) % 10;
    }
    if (e5 > 0) {
        res = (res * 5) % 10;
    }

    return res;
}
