/*
 * A216051  Even numbers m that have an odd number of Goldbach partitions
 *          whose lesser and greater elements each sum to a prime.
 * https://oeis.org/A216051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 380, u32 n <= 380, u64 n <= 380,
 *            u128 n <= 380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A216051.
   Even numbers m that have an odd number of Goldbach partitions
   such that the sum of the lesser elements over all partitions
   is prime, and the sum of the greater elements over all partitions
   is prime. */

/* -------------------------------------------------------------------------- */
/* Common includes */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------------- */
/* Forward declarations */

static bool is_prime_uint64(uint64_t n);
static void build_terms(idx_t limit, seq_t **terms, size_t *len);

/* -------------------------------------------------------------------------- */
/* External linkage function required by the framework */

seq_t a(idx_t n)
{
    /* Static state: cache of computed terms and the limit used to generate them */
    static idx_t        current_limit = 0;   /* highest even number considered */
    static seq_t       *cache_terms   = NULL;
    static size_t       cache_len     = 0;  /* number of terms in cache */
    static bool         built         = false;

    /* The sequence has offset 1; n == 0 is undefined – return 0 */
    if (n == 0) return 0;

    /* First time or need more terms than currently cached */
    if (!built || n > cache_len) {
        /* Decide a new limit – start with 32 if none, otherwise double until enough */
        idx_t limit = current_limit;
        if (limit == 0) limit = 32;
        while (true) {
            /* Free old cache before rebuilding */
            free(cache_terms);
            cache_terms = NULL;
            cache_len = 0;

            build_terms(limit, &cache_terms, &cache_len);

            if (cache_len >= (size_t)n) {
                built = true;
                current_limit = limit;
                break;
            }
            /* Not enough terms – increase limit */
            limit <<= 1;
        }
    }

    /* Term at index n is the (n-1)-th element of the cache (1‑based) */
    return cache_terms[n - 1];
}

/* -------------------------------------------------------------------------- */
/* Helper: trial‑division primality test for uint64_t */

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)           return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0)  return false;
    if (n % 3 == 0)        return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------------- */
/* Build all qualifying even numbers up to 'limit' and store them in an array.
   The caller supplies pointers where the result array and its length are returned. */

static void build_terms(idx_t limit, seq_t **terms, size_t *len)
{
    /* Allocate primality sieve */
    bool *is_composite = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite) { *terms = NULL; *len = 0; return; }

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes and store them */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++prime_cnt;

    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_composite); *terms = NULL; *len = 0; return;
    }
    size_t pi = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) primes[pi++] = (uint64_t)i;

    /* Arrays to count partitions and accumulate sums of lower/upper primes */
    uint16_t *cnt   = (uint16_t*)calloc((size_t)(limit + 1), sizeof(uint16_t));
    uint64_t *sumL  = (uint64_t*)calloc((size_t)(limit + 1), sizeof(uint64_t));
    uint64_t *sumU  = (uint64_t*)calloc((size_t)(limit + 1), sizeof(uint64_t));
    if (!cnt || !sumL || !sumU) {
        free(is_composite); free(primes); free(cnt); free(sumL); free(sumU);
        *terms = NULL; *len = 0; return;
    }

    /* Enumerate all unordered prime pairs (p <= q) with p+q <= limit */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t m = p + q;
            if (m > (uint64_t)limit) break;
            if ((m & 1ULL) == 0) {
                ++cnt[m];
                sumL[m] += p;
                sumU[m] += q;
            }
        }
    }

    /* Collect qualifying even numbers */
    size_t cap = 4096;
    seq_t *buffer = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!buffer) {
        free(is_composite); free(primes); free(cnt); free(sumL); free(sumU);
        *terms = NULL; *len = 0; return;
    }

    size_t tlen = 0;
    for (idx_t m = 4; m <= limit; m += 2) {
        if (cnt[m] & 1) {                     /* odd number of partitions */
            uint64_t low  = sumL[m];
            uint64_t high = sumU[m];
            if (!is_prime_uint64(low) || !is_prime_uint64(high))
                continue;
            /* store m */
            if (tlen == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(buffer, cap * sizeof(seq_t));
                if (!tmp) break;   /* allocation failed – stop */
                buffer = tmp;
            }
            buffer[tlen++] = (seq_t)m;
        }
    }

    /* Finalize */
    *terms = buffer;
    *len   = tlen;

    /* Free temporary arrays */
    free(is_composite);
    free(primes);
    free(cnt);
    free(sumL);
    free(sumU);
}
