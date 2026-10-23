/*
 * A185724  Primes that are not the sum of distinct primes with prime
 *          subscripts.
 * https://oeis.org/A185724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A185724: primes that are not the sum of distinct primes with prime subscripts. */
/* The sequence is finite; all primes > 96 are excluded because every integer > 96 is representable. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_prime_small(idx_t n)
{
    if (n < 2)
        return false;
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static storage for the computed sequence */
    static seq_t *terms   = NULL;
    static idx_t term_cnt = 0;

    if (n == 0)
        return (seq_t)0;    /* undefined for n == 0 */

    /* build the sequence only once */
    if (terms == NULL)
    {
        /* Generate all primes up to 96 */
        const idx_t LIMIT = 96;
        uint64_t primes_local[25];    /* 25 primes <= 96 */
        size_t prime_cnt = 0;
        for (uint64_t p = 2; p <= LIMIT; ++p) {
            bool prime = true;
            for (uint64_t d = 2; d * d <= p; ++d)
                if (p % d == 0) { prime = false; break; }
            if (prime)
                primes_local[prime_cnt++] = p;
        }

        /* Mark indices that are prime (1‑based) */
        bool idx_is_prime[26] = {0};
        for (size_t i = 2; i <= prime_cnt; ++i)
            idx_is_prime[i] = is_prime_small((idx_t)i);

        /* DP table for subset sums of primes with prime subscripts */
        bool representable[97] = {0};
        representable[0] = true;
        for (size_t i = 1; i <= prime_cnt; ++i) {
            if (!idx_is_prime[i])
                continue;        /* index not prime → skip */
            uint64_t val = primes_local[i - 1];
            if (val > LIMIT)
                continue;        /* value > 96 cannot be used */
            for (int s = LIMIT; s >= (int)val; --s)
                if (representable[s - (int)val])
                    representable[s] = true;
        }

        /* Collect primes <= 96 that are not representable */
        terms = (seq_t *)calloc(prime_cnt, sizeof(seq_t));
        if (terms != NULL) {
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes_local[i];
                if (p > LIMIT)
                    continue;        /* beyond the bound */
                if (!representable[(int)p])
                    terms[term_cnt++] = (seq_t)p;
            }
        }
    }

    if (n <= term_cnt)
        return terms[n - 1];
    return (seq_t)0;    /* n exceeds the length of the finite sequence */
}
