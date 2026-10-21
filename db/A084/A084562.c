/*
 * A084562  Even numbers having no more than two partitions into two odd
 *          primes.
 * https://oeis.org/A084562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084562: even numbers with at most two representations as sum of two odd primes. */

#include <stdlib.h>

static uint8_t *is_prime = NULL;          /* bitset of primality (0/1) */
static uint64_t *primes = NULL;           /* array of odd primes */
static size_t primes_count = 0;           /* number of primes stored */
static uint64_t sieve_limit = 0;          /* highest number currently sieved */

static seq_t *cache = NULL;               /* cached sequence terms */
static size_t cache_size = 0;
static size_t cache_capacity = 0;         /* capacity of cache array */

/* ------------------------------------------------------------------
   Simple sieve of Eratosthenes up to 'limit', storing odd primes only.
   ------------------------------------------------------------------ */
static void generate_sieve(uint64_t limit) {
    /* Allocate and initialize the primality array */
    uint8_t *tmp = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!tmp) return;                          /* allocation failure: abort silently */
    for (uint64_t i = 0; i <= limit; ++i) tmp[i] = 1;
    if (limit >= 0) tmp[0] = 0;
    if (limit >= 1) tmp[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (tmp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                tmp[j] = 0;
        }
    }

    /* Count odd primes to allocate 'primes' array */
    size_t count = 0;
    for (uint64_t i = 3; i <= limit; i += 2) if (tmp[i]) ++count;

    uint64_t *new_primes = (uint64_t *)realloc(primes, count * sizeof(uint64_t));
    if (!new_primes) return;                 /* allocation failure */
    primes = new_primes;
    size_t idx = 0;
    for (uint64_t i = 3; i <= limit; i += 2) {
        if (tmp[i]) primes[idx++] = i;
    }
    primes_count = count;
    sieve_limit = limit;

    /* Replace old primality array */
    free(is_prime);
    is_prime = tmp;
}

/* ------------------------------------------------------------------
   Ensure that the sieve covers at least 'limit'.
   ------------------------------------------------------------------ */
static void ensure_sieve(uint64_t limit) {
    if (limit <= sieve_limit) return;
    /* Rebuild the entire sieve for the enlarged limit */
    generate_sieve(limit);
}

/* ------------------------------------------------------------------
   Count unordered pairs of odd primes summing to 'n'.
   Returns the exact count or a value >2 (early exit).
   ------------------------------------------------------------------ */
static int goldbach_pairs_count(uint64_t n) {
    int count = 0;
    uint64_t half = n >> 1;          /* n/2 */
    for (size_t i = 0; i < primes_count && primes[i] <= half; ++i) {
        uint64_t p = primes[i];
        uint64_t q = n - p;          /* q is odd because n and p are odd/odd */
        if (q <= sieve_limit && is_prime[q]) {
            ++count;
            if (count > 2) return count;
        }
    }
    return count;
}

/* ------------------------------------------------------------------
   Store 'value' into the cached sequence (dynamic array).
   ------------------------------------------------------------------ */
static void cache_term(uint64_t value) {
    if (cache_size == cache_capacity) {
        cache_capacity = cache_capacity ? cache_capacity << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        if (!tmp) return;            /* allocation failure */
        cache = tmp;
    }
    cache[cache_size++] = (seq_t)value;
}

/* ------------------------------------------------------------------
   Return the nth term of A084562 (1‑based index).
   ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined offset (safe) */

    if (!cache) {
        cache_capacity = 8;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache_size = 0;
    }

    static uint64_t last_checked_even = 0;      /* last even number examined */

    while (cache_size < (size_t)n) {
        uint64_t candidate = last_checked_even ? last_checked_even + 1 : 2;
        if (candidate & 1ULL) ++candidate;     /* make it even */

        /* Ensure primes up to the candidate are available */
        ensure_sieve(candidate);

        int cnt = goldbach_pairs_count(candidate);
        if (cnt <= 2) cache_term(candidate);

        last_checked_even = candidate;
    }
    return cache[n - 1];
}
