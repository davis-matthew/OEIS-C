/*
 * A033716  Number of integer solutions to the equation x^2 + 3y^2 = n.
 * https://oeis.org/A033716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer solutions of x² + 3y² = n
 * Range:     u8 n <= 9456, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n, enforce the known arithmetic rule: */
/*  • every prime ≡ 2 (mod 3) must appear with even exponent;  */
/*  • the exponent of 2 must be even (allowing 0);          */
/*  • let B = ∏ (e_i+1) over primes ≡ 1 (mod 3)          */
/*  • if n is odd (e₂=0) return 2·B, else return 6·B.      */
/*  For n = 0, return 1.                                        */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    uint64_t u = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(u, primes, exps, &cnt);

    seq_t B = 1;
    unsigned e2 = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            e2 = (unsigned)e;
            if (e % 2 == 1) return 0;          /* 2 exponent must be even */
        } else if ((p % 3) == 2) {
            if (e % 2 == 1) return 0;          /* primes 2 mod 3 must have even exponent */
        } else if ((p % 3) == 1) {
            B *= ((seq_t)e + 1);                /* contributions of 1 mod 3 primes */
        } /* p == 3 is ignored */
    }

    /* If the exponent of 2 is odd (should have been caught above), no solution */
    if (e2 % 2 == 1) return 0;

    if (e2 == 0)      /* n is odd */
        return (seq_t)2 * B;
    else              /* n divisible by 4 (exponent of 2 even and >0) */
        return (seq_t)6 * B;
}
