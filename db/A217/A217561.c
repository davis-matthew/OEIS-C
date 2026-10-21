/*
 * A217561  The only prime p such that 3a < p < 3b where a, b are consecutive
 *          primes.
 * https://oeis.org/A217561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217561: For consecutive primes a<b the unique prime p
 * in the interval [3a , 3b] (if it exists) is recorded.
 * a(n) is the nth such prime (a(1)=7). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the DFA type used by the harness */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Prime sieve: build once up to a preset limit.                     */
/* ------------------------------------------------------------------ */
#define PRIME_SIEVE_LIMIT      20000000U      /* 20 million */
static uint32_t *prime_list = NULL;           /* array of primes          */
static size_t   prime_count = 0;              /* number of primes found   */
static bool     sieve_built = false;

/* simple binary search helpers on the prime array */
static size_t lower_bound_idx(uint32_t *arr, size_t n, uint32_t val)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m] < val) l = m + 1;
        else r = m;
    }
    return l;
}

static size_t upper_bound_idx(uint32_t *arr, size_t n, uint32_t val)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m] <= val) l = m + 1;
        else r = m;
    }
    return l;
}

/* build the sieve and prime list */
static void build_prime_sieve(void)
{
    if (sieve_built) return;
    const uint32_t limit = PRIME_SIEVE_LIMIT;
    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_composite) abort(); /* allocation failed */

    prime_count = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* prime found */
            ++prime_count;
            /* mark multiples */
            if (i <= limit / i) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    /* allocate prime list */
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) abort();
    /* fill the list */
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) prime_list[idx++] = i;
    free(is_composite);
    sieve_built = true;
}

/* ------------------------------------------------------------------ */
/* Term storage: keep generated sequence terms in a dynamic array.   */
/* ------------------------------------------------------------------ */
static seq_t *term_cache = NULL;
static size_t term_len   = 0;
static size_t term_cap   = 0;
static size_t pair_pos   = 0;   /* index in prime_list of the left prime a */

/* Ensure we have at least n terms in the cache */
static void ensure_terms(idx_t n)
{
    if (n <= (idx_t)term_len) return;
    build_prime_sieve();

    /* enlarge cache if needed */
    if (term_cap < 8) {
        term_cap = 8;
        term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
    }

    while (term_len < (size_t)n && pair_pos + 1 < prime_count) {
        uint32_t a_prime = prime_list[pair_pos];
        uint32_t b_prime = prime_list[pair_pos + 1];

        /* interval (3*a_prime, 3*b_prime)            */
        uint32_t lo = 3U * a_prime;
        uint32_t hi = 3U * b_prime;

        /* locate primes in that interval */
        size_t low_idx  = upper_bound_idx(prime_list, prime_count, lo);
        size_t high_idx = lower_bound_idx(prime_list, prime_count, hi);
        size_t count = 0;
        if (high_idx > low_idx) count = high_idx - low_idx;

        if (count == 1) {
            /* the unique prime is the one at low_idx */
            seq_t val = (seq_t)prime_list[low_idx];
            if (term_len == term_cap) {
                term_cap <<= 1;
                term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
            }
            term_cache[term_len++] = val;
        }
        ++pair_pos;
    }
}

/* ------------------------------------------------------------------ */
/* Public function implementing the sequence.                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;          /* behaviour below offset is irrelevant   */
    ensure_terms(n);
    return term_cache[n - 1];
}
