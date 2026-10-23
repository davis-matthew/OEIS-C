/*
 * A118081  Even numbers that can't be represented as the sum of two odd
 *          composite numbers.
 * https://oeis.org/A118081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   sequence by search – even integers that cannot be written * as
 *            the sum of two odd composite numbers. The function a(idx_t n) *
 *            returns the n‑th term (a(1)=2). Internally it builds a sieve of
 *            * composites, keeps a list of odd composites and tests each even
 *            * number until the requested index is rea...
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state shared by all calls --------------------------------------- */
static uint8_t *prime_mask = NULL;     /* 1= composite, 0=prime */
static size_t prime_mask_cap = 0;      /* capacity of prime_mask */

static uint64_t *odd_comps = NULL;     /* odd composites ≥9 */
static size_t odd_comps_size = 0;
static size_t odd_comps_cap = 0;

static seq_t *seq_cache = NULL;        /* cache of answer terms */
static size_t seq_cache_size = 0;
static size_t seq_cache_cap = 0;

static uint64_t current_even = 2;      /* smallest even number not yet tested */

/* Ensure prime_mask and odd_comps cover up to at least `limit`. ---------- */
static void ensure_limit(uint64_t limit)
{
    if (prime_mask_cap > limit)
        return;                       /* already big enough */

    /* Release previous data */
    free(prime_mask);
    free(odd_comps);

    /* Allocate new prime/composite mask */
    prime_mask_cap = limit + 1;
    prime_mask = (uint8_t*)calloc(prime_mask_cap, sizeof(uint8_t));
    if (prime_mask == NULL)
        abort();

    if (prime_mask_cap >= 2) {
        prime_mask[0] = 1;            /* 0 not prime */
        prime_mask[1] = 1;            /* 1 not prime */
    }

    /* Sieve of Eratosthenes */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (prime_mask[i] == 0) {      /* i is prime */
            for (uint64_t j = i * i; j <= limit; j += i)
                prime_mask[j] = 1;     /* mark composite */
        }
    }

    /* Build list of odd composite numbers ≥9 */
    odd_comps_size = 0;
    odd_comps_cap  = 0;
    odd_comps = NULL;

    for (uint64_t i = 9; i <= limit; i += 2) {
        if (prime_mask[i]) {           /* composite */
            if (odd_comps_size >= odd_comps_cap) {
                size_t new_cap = odd_comps_cap == 0 ? 16 : odd_comps_cap << 1;
                uint64_t *tmp = (uint64_t*)realloc(odd_comps, new_cap * sizeof(uint64_t));
                if (tmp == NULL)
                    abort();
                odd_comps = tmp;
                odd_comps_cap = new_cap;
            }
            odd_comps[odd_comps_size++] = i;
        }
    }
}

/* Test whether an even number can be expressed as the sum of two odd
 * composite numbers.  Uses the pre‑computed odd_comps list and prime_mask. */
static bool is_representable(uint64_t even)
{
    for (size_t idx = 0; idx < odd_comps_size; ++idx) {
        uint64_t a = odd_comps[idx];
        if (a > even - a)          /* a > even/2 */
            break;
        uint64_t b = even - a;     /* b is odd too */
        if (b >= 9 && prime_mask[b])   /* b is odd composite */
            return true;
    }
    return false;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Compute terms until we have n of them in the cache. */
    while (seq_cache_size < (size_t)n) {
        ensure_limit(current_even);               /* prepare sieve data */

        if (!is_representable(current_even)) {
            /* Append to cache */
            if (seq_cache_size >= seq_cache_cap) {
                size_t new_cap = seq_cache_cap == 0 ? 16 : seq_cache_cap << 1;
                seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    abort();
                seq_cache = tmp;
                seq_cache_cap = new_cap;
            }
            seq_cache[seq_cache_size++] = (seq_t)current_even;
        }
        current_even += 2;          /* next even number */
    }
    return seq_cache[(size_t)n - 1];
}
