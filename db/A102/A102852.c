/*
 * A102852  Primes whose squares are congruent to 5 (modulo 19).
 * https://oeis.org/A102852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p with p^2 ≡ 5 (mod 19) (search/sieve)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* static storage for generated primes */
static seq_t *primes      = NULL;
static idx_t  prime_len   = 0;
static idx_t  prime_cap   = 0;
static uint64_t next_candidate = 2;

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour below offset is irrelevant */
    if (n == 0) return 0;

    /* generate primes until we have at least n of them */
    while (prime_len < n)
    {
        /* find the next candidate with residue 9 or 10 modulo 19 */
        do { ++next_candidate; }
        while (next_candidate % 19 != 9 && next_candidate % 19 != 10);

        if (is_prime_u64(next_candidate))
        {
            /* grow array if necessary */
            if (prime_len == prime_cap)
            {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                primes = seq_realloc(primes, prime_cap);
            }
            primes[prime_len] = (seq_t)next_candidate;
            ++prime_len;
        }
    }

    return primes[n - 1];
}
