/*
 * A235872  Number of solutions to the equation x^2=0 in the ring of Gaussian
 *          integers modulo n.
 * https://oeis.org/A235872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of p^(e if p=2
 *            else 2*floor(e/2))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided seq_mul utility for arbitrary‑precision multiplication. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

/* Number of solutions to x² ≡ 0 (mod n) in ℤ[i] */
seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* trial‑division factorisation */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* exponent to use in the product */
        idx_t exp = (p == 2) ? e : (e &~(idx_t)1);  /* 2*floor(e/2) */

        if (exp == 0)
            continue;   /* factor contributes 1 */

        /* compute p^exp using seq_mul */
        seq_t val = (seq_t)1;
        for (idx_t k = 0; k < exp; ++k)
            val = seq_mul(val, (seq_t)p);

        result = seq_mul(result, val);
    }

    /* handle remaining prime factor > sqrt(m) */
    if (m > 1) {
        idx_t p = (idx_t)m;
        idx_t exp = (p == 2) ? 1 : 0;  /* p^1 yields exp=0 for odd primes */

        if (exp != 0) {
            seq_t val = (seq_t)p;  /* p^1 */
            result = seq_mul(result, val);
        }
    }

    return result;
}
