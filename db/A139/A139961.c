/*
 * A139961  Primes of the form 17x^2+24y^2.
 * https://oeis.org/A139961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, in_residue_class
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A139961: Primes of the form 17x^2+24y^2
   a(n) = the nth prime p with p = 17x^2 + 24y^2
   (equivalently p ≡ 17,41,65,113,209,233,329,377,401 (mod 408)). */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/in_residue_class.h"

static seq_t *prime_cache = NULL;    /* cached primes of the desired form */
static size_t prime_cache_len = 0;    /* number of primes stored */
static idx_t  cache_limit  = 0;       /* sieve upper bound used last time */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* fill prime_cache until it contains at least 'target' primes.       */
static void build_cache(idx_t target)
{
    if (prime_cache_len >= (size_t)target) return;   /* already enough */

    /* start from the previously used limit or 1 000 if none yet */
    idx_t limit = cache_limit ? cache_limit : 1000;

    while (1) {
        /* allocate the sieve array (bool is 1 byte) */
        size_t sz = (size_t)limit + 1;
        bool *is_prime = (bool*)malloc(sz);
        if (!is_prime) return;                     /* serious error – give up */

        /* initialise the sieve */
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i < sz; ++i) is_prime[i] = true;

        /* simple Eratosthenes */
        for (idx_t i = 2; (i * i) <= limit; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[(size_t)j] = false;
            }
        }

        /* count primes of the desired form to preallocate the array */
        size_t temp_len = 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (is_prime[(size_t)p] && in_residue_class((seq_t)p))
                ++temp_len;
        }

        /* allocate array of seq_t using the provided helper */
        seq_t *temp = alloc_seq_array(temp_len);
        if (!temp) {
            free(is_prime);
            return;    /* allocation failure */
        }

        /* collect the primes */
        size_t idx = 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (is_prime[(size_t)p] && in_residue_class((seq_t)p)) {
                temp[idx++] = (seq_t)p;
            }
        }

        free(is_prime);

        /* free the previous cache if any, using the matching helper */
        if (prime_cache) free_seq_array(prime_cache);

        /* install the new cache */
        prime_cache      = temp;
        prime_cache_len  = temp_len;
        cache_limit      = limit;

        if (prime_cache_len >= (size_t)target) break;

        /* not enough yet – extend the search limit and retry */
        if (limit > (idx_t)(-1) / 2) break;   /* avoid overflow */
        limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, return 0 */
    build_cache(n);
    return prime_cache[n - 1];
}
