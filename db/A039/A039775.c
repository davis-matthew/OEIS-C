/*
 * A039775  Number of new fractions m/n, where m is prime, (m,n)=1 and "new"
 *          means the value of mn has not occurred before.
 * https://oeis.org/A039775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes m ≤ n with gcd(m,n)=1 such that the *
 *            product m*n has not occurred as m'*n' for any prime m' and any *
 *            denominator n' < n. The implementation keeps a bitset of seen *
 *            products and a list of primes up to the current maximum n,
 *            updating * both lazily when a(n) is requested....
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   bit_test
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/bit_test.h"

/*--- Global state -------------------------------------------------------*/
static seq_t *a_cache = NULL;      /* a_cache[n] = a(n), 1‑based index */
static idx_t  cache_size = 0;      /* number of entries allocated (max n) */

static uint64_t *prod_bits = NULL;   /* bitset of seen products */
static size_t   prod_bits_words = 0; /* number of 64‑bit words used */

static uint64_t *primes = NULL;      /* primes up to current limit */
static size_t   prime_cnt = 0;
static idx_t    prime_limit = 0;    /* largest n for which primes are known */

static inline void bit_set(uint64_t *bits, uint64_t pos)
{
    bits[pos >> 6] |= (1ULL << (pos & 63));
}

/*--- Ensure prod_bits can index up to new_limit² ------------------------*/
static void ensure_product_bits(idx_t new_limit)
{
    uint64_t limit_sq = (uint64_t)new_limit * (uint64_t)new_limit;
    size_t needed = (size_t)(limit_sq >> 6) + 1;   /* 64 bits per word */
    if (needed <= prod_bits_words) return;
    uint64_t *tmp = (uint64_t*)realloc(prod_bits, needed * sizeof(uint64_t));
    if (!tmp) return; /* keep old on failure */
    for (size_t i = prod_bits_words; i < needed; ++i) tmp[i] = 0ULL;
    prod_bits = tmp;
    prod_bits_words = needed;
}

/*--- Prime sieve up to `limit` ------------------------------------------*/
static void build_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;   /* already sufficient */
    free(primes);
    primes = NULL;
    prime_cnt = 0;
    prime_limit = limit;

    /* Boolean sieve (0..limit) */
    char *is_prime = (char*)calloc((size_t)(limit + 1), 1);
    if (!is_prime) return;              /* allocation failure */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    free(is_prime);
}

/*--- Compute a(n) up to target ------------------------------------------*/
static void compute_up_to(idx_t target)
{
    if (target < 1) return;
    if (target <= cache_size) return;    /* already computed */

    idx_t old_max = cache_size;          /* last computed n */

    /* Resize cache to hold indices up to target */
    seq_t *tmp_cache = (seq_t*)realloc(a_cache, (target + 1) * sizeof(seq_t));
    if (!tmp_cache) return;              /* allocation failure */
    for (idx_t i = old_max + 1; i <= target; ++i) tmp_cache[i] = 0; /* init */
    a_cache = tmp_cache;
    cache_size = target;

    /* Ensure product bitset and primes are ready */
    ensure_product_bits(target);
    build_primes_upto(target);

    /* Compute values for n = old_max+1 .. target */
    for (idx_t n = old_max + 1; n <= target; ++n) {
        seq_t cnt = 0;
        for (size_t pi = 0; pi < prime_cnt && primes[pi] <= n; ++pi) {
            uint64_t p = primes[pi];
            if (n % p == 0) continue;    /* not coprime */
            uint64_t prod = p * (uint64_t)n;
            if (!bit_test(prod_bits, prod)) {
                bit_set(prod_bits, prod);
                ++cnt;
            }
        }
        a_cache[n] = cnt;
    }
}

/*--- Public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    compute_up_to(n);
    return a_cache[n];
}
