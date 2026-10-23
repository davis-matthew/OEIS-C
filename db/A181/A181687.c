/*
 * A181687  Numbers k such that the number of odd divisors of (2k)^2 is an odd
 *          divisor of (2k)^2, and the number of even divisors of (2k)^2 is an
 *          even divisor of (2k)^2.
 * https://oeis.org/A181687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 8573, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search of k satisfying the divisor conditions
   described in A181687. */
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, just safety */

    /* Static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t next_k = 1; /* next candidate k to test */

    /* Expand cache until we have at least n terms */
    while (terms_len < (size_t)n)
    {
        uint64_t k = next_k++;
        /* Decompose k into 2^e * m (m odd) */
        uint64_t temp = k, e = 0;
        while ((temp & 1ULL) == 0ULL) { e++; temp >>= 1; }
        uint64_t m = temp;   /* odd part */

        /* Factor m */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* Compute P = product (2*exponent + 1) */
        seq_t P = 1;
        for (size_t i = 0; i < cnt; ++i)
            P *= (seq_t)(2 * exps[i] + 1);

        /* Compute m^2 as seq_t */
        seq_t m2 = (seq_t)m * (seq_t)m;
        if (m2 % P != 0) continue;  /* P must divide m^2 */

        seq_t X = m2 / P;           /* X = m^2 / P */

        /* Compute u = odd part of (1 + e) */
        uint64_t one_plus_e = 1 + e;
        uint64_t u = one_plus_e;
        while ((u & 1ULL) == 0ULL) u >>= 1;

        if (X % (seq_t)u != 0) continue;  /* Extra condition for even divisors */

        /* k satisfies the conditions; add to cache */
        if (terms_len == terms_cap)
            grow_seq_array(&terms, &terms_cap, terms_cap ? terms_cap * 2 : 4);
        terms[terms_len++] = (seq_t)k;
    }

    return terms[n - 1];
}
