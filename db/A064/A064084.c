/*
 * A064084  A multiplicative version of 2^n - 1 (A000225).
 * https://oeis.org/A064084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   _factor_u64, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Π_{p^e‖n} (2^(p^e) – 1)
 *
 * This implementation factors n using the helper _factor_u64, then for
 * each prime power p^e computes the term 2^(p^e) – 1 with an exact
 * integer type (seq_t).  The result is the product of all these terms.
 *
 * Dependencies:
 *   - ../utils/_factor_u64.h   : factorization of uint64_t numbers
 *   - ../utils/big_pow.h       : arbitrary‑precision exponentiation
 */
#include "../utils/_factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is a 1‑based index; 0 is not used, but handle gracefully */
    if (n == 0)
        return 0;

    /* Convert to native type for factorization */
    uint64_t m = (uint64_t)n;

    /* Storage for prime factors and exponents */
    uint64_t pr[64], exp[64];
    size_t cnt = 0;

    /* Factorize m */
    _factor_u64(m, pr, exp, &cnt);

    seq_t prod = 1;    /* result accumulator */

    for (size_t i = 0; i < cnt; ++i) {
        /* Compute primePower = p^e as seq_t to avoid overflow */
        seq_t primePower = 1;
        for (uint64_t j = 0; j < exp[i]; ++j)
            primePower = primePower * (seq_t)pr[i];

        /* term = 2^(primePower) – 1 */
        seq_t term = big_pow((seq_t)2, primePower);
        term -= 1;

        /* multiply into result */
        prod = prod * term;
    }

    return prod;
}
