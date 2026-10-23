/*
 * A167488  Primes p such that (p reversed)-6 is also a prime.
 * https://oeis.org/A167488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 279, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that reverse(p) – 6 is also prime (OEIS A167488) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Cached qualifying primes */
static seq_t *qual_primes = NULL;   /* array of qualifying primes   */
static size_t  qual_cap   = 0;      /* current array capacity       */
static size_t  qual_len   = 0;      /* number of stored primes      */
static seq_t  next_prime_candidate = 2; /* next integer to test       */

/* Ensure that the cache contains at least 'num' qualifying primes */
static void ensure_cache(idx_t num)
{
    /* Initialise cache on first use */
    if (qual_primes == NULL) {
        qual_cap   = 16;
        qual_primes = seq_alloc_array(qual_cap);
        qual_len   = 0;
        next_prime_candidate = 2;
    }

    while (qual_len < (size_t)num) {
        seq_t cand = next_prime_candidate++;
        /* Skip non‑primes */
        if (!is_prime_big(cand))
            continue;

        seq_t rev = reverse_decimal(cand);
        /* rev must be > 6 so that rev - 6 is non‑negative */
        if (rev <= 6)
            continue;

        seq_t cand2 = rev - 6;
        if (!is_prime_big(cand2))
            continue;

        /* Grow the array if needed */
        if (qual_len == qual_cap) {
            size_t new_cap = qual_cap << 1;          /* double */
            seq_t *new_arr = seq_alloc_array(new_cap);
            for (size_t i = 0; i < qual_len; ++i)
                new_arr[i] = qual_primes[i];
            seq_free_array(qual_primes);
            qual_primes = new_arr;
            qual_cap   = new_cap;
        }

        /* Store qualifying prime */
        qual_primes[qual_len++] = cand;
    }
}

/* External function required by the testing harness */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – undefined for n = 0 */
        return (seq_t)0;
    ensure_cache(n);
    return qual_primes[n - 1];
}
