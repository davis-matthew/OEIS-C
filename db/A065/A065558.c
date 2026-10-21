/*
 * A065558  Let G_n be the group of invertible 2 X 2 matrices mod n (sequence
 *          A000252). a(n) is the maximal degree of an irreducible
 *          representation of G_n.
 * https://oeis.org/A065558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, mult
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065558 – maximal degree of an irreducible representation of GL(2,ℤ/nℤ)
   Formula: a(n) = ∏_{p^k‖n} f(p,k) where
     f(2,1) = 2,
     f(2,k) = 3·2^{k-1}  (k ≥ 2),
     f(p,k) = p^k + p^{k-1}  (odd prime p).  The function is multiplicative. */
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) {
            res *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1; dummy value */

    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t k = exps[i];
        seq_t val;

        if (p == 2) {
            if (k == 1) {
                val = 2;
            } else {
                seq_t pow2 = pow_seq((seq_t)2, k - 1);
                val     = (seq_t)3 * pow2;
            }
        } else {
            seq_t pk = pow_seq((seq_t)p, k);
            seq_t pk_minus = pk / (seq_t)p; /* exact division */
            val = pk + pk_minus;
        }
        result *= val;
    }
    return result;
}
