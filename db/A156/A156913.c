/*
 * A156913  Numbers n with property that A100486(n) is square.
 * https://oeis.org/A156913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156913: a(n) = the n‑th integer t such that pi(t)+p(t) is a perfect square,
   where pi(t) is the number of primes ≤ t and p(t) is the t‑th prime.  The
   implementation builds primes incrementally and stores the observed terms
   in a dynamically sized array.  The algorithm is O(N log N / log log N)
   for the first N terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Global dynamic state ---------- */
static bool initialized = false;

static uint8_t *sieve_bits = NULL;   /* bitset for odd composites */
static idx_t   sieve_limit = 0;      /* numeric limit of the sieve */

static uint32_t *primes = NULL;      /* array of primes (1‑based: primes[0]=2) */
static size_t primes_cnt = 0;        /* number of primes stored */

static uint64_t *terms = NULL;      /* array of terms, terms[0] = first term */
static size_t terms_cap   = 0;       /* allocated capacity of terms[] */
static size_t terms_cnt   = 0;       /* number of terms stored (no dummy) */

static idx_t   next_candidate = 2;   /* next number to test (start at 2) */
static idx_t   pi_count       = 0;   /* pi(next_candidate-1) */

/* ---------- Helper prototypes ---------- */
static void init(void);
static inline bool is_composite_odd(idx_t n);
static void ensure_terms_capacity(size_t needed);
static void ensure_prime_list(size_t needed);
static void grow_sieve_to(idx_t limit);
static uint64_t isqrt_uint64(uint64_t x);

/* ---------- Implementation ---------- */

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Seed a very small sieve so that the first term can be found. */
    grow_sieve_to(16);               /* contains primes up to 16 */

    /* Allocate first terms array with some capacity. */
    terms_cap = 64;
    terms = (uint64_t *)malloc(terms_cap * sizeof(uint64_t));
    if (!terms) exit(1);
    terms_cnt = 0;                   /* no terms yet */

    next_candidate = 2;
    pi_count = 0;
}

/* Return true if odd n ≥3 is marked composite in the current sieve. */
static inline bool is_composite_odd(idx_t n)
{
    /* n is odd, n >= 3 */
    idx_t idx = n >> 1;                      /* 3->1, 5->2, ... */
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1;
}

/* Grow the terms array to at least the specified capacity. */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that the prime list contains at least 'needed' primes
   (i.e., primes[needed-1] is valid).  This expands the sieve as required. */
static void ensure_prime_list(size_t needed)
{
    if (primes_cnt >= needed) return;
    idx_t limit = sieve_limit ? sieve_limit : 2;
    while (primes_cnt < needed) {
        if (limit < 16) limit = 16;
        else limit <<= 1;
        grow_sieve_to(limit);
    }
}

/* Expand the sieve to cover all numbers ≤ limit and rebuild the prime list. */
static void grow_sieve_to(idx_t limit)
{
    if (limit <= sieve_limit) return;        /* already sufficient */

    idx_t bits_needed = (limit >> 1) + 1;      /* number of odd numbers */

    size_t bytes_needed = (bits_needed + 7) >> 3;
    uint8_t *new_bits = (uint8_t *)calloc(bytes_needed, 1);
    if (!new_bits) exit(1);

    /* Run Eratosthenes on odd numbers only. */
    for (idx_t i = 3; i * i <= limit; i += 2) {
        if ((new_bits[(i >> 1) >> 3] >> ((i >> 1) & 7)) & 1) continue; /* composite */
        for (idx_t j = i * i; j <= limit; j += i << 1) {
            new_bits[(j >> 1) >> 3] |= 1 << ((j >> 1) & 7);
        }
    }

    /* Count primes and build new primes array. */
    size_t cnt = 0;
    if (limit >= 2) ++cnt;                      /* prime 2 */
    for (idx_t i = 3; i <= limit; i += 2)
        if (!((new_bits[(i >> 1) >> 3] >> ((i >> 1) & 7)) & 1)) ++cnt;

    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);

    size_t pos = 0;
    if (limit >= 2) new_primes[pos++] = 2;
    for (idx_t i = 3; i <= limit; i += 2)
        if (!((new_bits[(i >> 1) >> 3] >> ((i >> 1) & 7)) & 1))
            new_primes[pos++] = (uint32_t)i;

    free(sieve_bits);
    free(primes);
    sieve_bits = new_bits;
    primes   = new_primes;
    sieve_limit = limit;
    primes_cnt  = pos;
}

/* Integer square root for uint64_t using binary search with 128‑bit arithmetic. */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    init();

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        /* Ensure we have enough primes to index p(next_candidate). */
        ensure_prime_list((size_t)next_candidate);

        /* Ensure candidate is within current sieve. */
        if (next_candidate > sieve_limit)
            grow_sieve_to(next_candidate);

        /* Test whether next_candidate is prime. */
        bool is_prime = false;
        if (next_candidate == 2) {
            is_prime = true;
        } else if ((next_candidate & 1) && !is_composite_odd(next_candidate)) {
            is_prime = true;
        }

        if (is_prime) ++pi_count;

        /* pi(next_candidate) is pi_count; p(next_candidate) is primes[next_candidate-1]. */
        uint64_t sum = pi_count + (uint64_t)primes[(size_t)next_candidate - 1];
        uint64_t r = isqrt_uint64(sum);
        if (r * r == sum) {
            /* Store the term. */
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt] = (uint64_t)next_candidate;
            ++terms_cnt;
        }

        ++next_candidate;
    }

    return (seq_t)terms[n - 1];
}
