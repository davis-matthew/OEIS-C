/*
 * A117660  Number of solutions to x^(k+3)=x^k mod n for some k>=1.
 * https://oeis.org/A117660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) computed via the known multiplicative definition for OEIS
 *            sequence A117660: a(1)=1, for n>1, factor n = ∏ p^e and if p==3:
 *            a(3^e)= 3^(e-1)+3 (with a(3)=2), if p≠3 and p≡1 (mod3):
 *            a(p^e)=p^(e-1)+3, if p≠3 and p≡2 (mod3): a(p^e)=p^(e-1)+1. Then
 *            a(n)=∏ a(p^e)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;

        if (p == (uint64_t)3) {
            if (e == (uint64_t)1)
                term = (seq_t)2;
            else
                term = pow_seq((seq_t)p, (int)(e - 1)) + (seq_t)3;
        } else if (p % (uint64_t)3 == (uint64_t)1) {
            term = pow_seq((seq_t)p, (int)(e - 1)) + (seq_t)3;
        } else {
            term = pow_seq((seq_t)p, (int)(e - 1)) + (seq_t)1;
        }

        result *= term;
    }

    return result;
}
