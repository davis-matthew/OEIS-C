/*
 * A102852  Primes whose squares are congruent to 5 (modulo 19).
 * https://oeis.org/A102852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p with p^2 ≡ 5 (mod 19) (search/sieve)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime64, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime64.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

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

        if (is_prime64(next_candidate))
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
