/*
 * A072084  In prime factorization of n replace all primes with their numbers
 *          of 1's in binary representation.
 * https://oeis.org/A072084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, mult
 * Formula:   a(n) = \prod_{p^e \| n} popcount(p)^e , with a(1) = 1
 * Range:     u8 n <= 6560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper _factor_u64 to obtain the prime factorisation of n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1; for n = 0 the sequence is undefined but we return 1 for safety. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;

    /* Factorise n.  The helper fills pr[], exp[] and cnt. */
    _factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = exp[i];
        /* popcount of the prime p.  GCC/Clang builtin works for 64‑bit types. */
        unsigned int pop = __builtin_popcountll(p);

        seq_t term = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k)
            term *= (seq_t)pop;

        result *= term;
    }
    return result;
}
