/*
 * A092949  Numbers of the form prime(n+1) + prime(n) + 1.
 * https://oeis.org/A092949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + prime(n+1) + 1 (offset 1)
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* External integer types supplied by the build */
extern union { uint64_t u; int64_t i; } dummy;  /* To suppress unused warnings */

/* Prime cache: prime_cache[0] = 2, prime_cache[1] = 3, ... */
static uint64_t *prime_cache = NULL;
static idx_t prime_cnt   = 0;

/* Enlarge the prime cache until it contains at least 'need' primes.
 * The implementation uses a simple sieve of Eratosthenes.  The limit
 * is increased geometrically until enough primes are found. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= need)
        return;

    /* Rough upper bound for the nth prime: p_n < n * 15  (valid for n ≤ 20000) */
    idx_t limit = need * 15;
    if (limit < 200)
        limit = 200;

    while (true) {
        /* Mark composites: 0 = prime, 1 = composite */
        bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
        if (!is_comp) {
            /* Allocation failure: abort the program (as required by the build). */
            exit(EXIT_FAILURE);
        }

        /* Temporary buffer large enough to hold all primes up to 'limit'.
           The number of primes ≤ limit is ≤ limit, so this allocation is safe. */
        uint64_t *tmp_buf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
        if (!tmp_buf) {
            free(is_comp);
            exit(EXIT_FAILURE);
        }

        uint64_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                tmp_buf[cnt++] = i;
                if (i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[j] = true;
                }
            }
        }

        free(is_comp);

        if (cnt >= need) {          /* We have enough primes */
            if (prime_cache)
                free(prime_cache);
            prime_cache = tmp_buf;
            prime_cnt   = cnt;
            return;
        }

        /* Not enough primes: try with a larger limit. */
        free(tmp_buf);
        limit <<= 1;                /* double the limit */
    }
}

/* Return the nth term of A092949.  n is 1‑based; a(1) = 6. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* Undefined for n = 0; behaviour outside the
                                   provided offset is irrelevant. */

    /* We need primes n and n+1 (1‑based), stored at indices n-1 and n. */
    ensure_primes(n + 1);

    seq_t p_n   = (seq_t)prime_cache[n - 1];  /* prime(n)   */
    seq_t p_np1 = (seq_t)prime_cache[n];      /* prime(n+1) */

    return p_n + p_np1 + (seq_t)1;            /* prime(n) + prime(n+1) + 1 */
}
