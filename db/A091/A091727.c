/*
 * A091727  Norms of prime ideals of Z[sqrt(-5)].
 * https://oeis.org/A091727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Norms of prime ideals in ℤ[√‑5] are given by * a(n) = the nth
 *            smallest of the set * { p (if p ≡ 1,2,3,5,7,9 (mod 20)) } U * {
 *            p² (if p ≡ 11,13,17,19 (mod 20)) } , * where the first term
 *            (n=1) is 2. * The sequence is sorted increasingly by the numeric
 *            value of the norm
 * Range:     u8 n <= 28, u16 n <= 3269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_split_prime, isqrt32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_split_prime.h"
#include "../utils/isqrt32.h"

/* ------------------------------------------------------------------ */
/* Parameters for precomputation                                    */
/* ------------------------------------------------------------------ */
#define MAX_LIMIT 3000000U   /* must be large enough for all required n */

/* ------------------------------------------------------------------ */
/* Global arrays and flags                                            */
/* ------------------------------------------------------------------ */
static uint8_t   *is_comp = NULL;          /* is composite */
static uint32_t *pref_cnt  = NULL;         /* number of primes ≤ i */
static uint32_t *pref_split = NULL;        /* number of split primes ≤ i */
static bool      data_init = false;

/* ------------------------------------------------------------------ */
/* Helper: integer square‑root for 32‑bit values                      */

/* ------------------------------------------------------------------ */
/* Test whether a prime p belongs to the “split / ramified” residue */
 /* classes 2,5,1,3,7,9 (mod 20).                                    */

/* ------------------------------------------------------------------ */
/* Precompute primes and prefix sums                                 */
/* ------------------------------------------------------------------ */
static void init_prime_data(void) {
    if (data_init) return;
    data_init = true;

    is_comp   = (uint8_t*)calloc((size_t)MAX_LIMIT + 1U, 1U);
    pref_cnt  = (uint32_t*)malloc((size_t)(MAX_LIMIT + 1U) * sizeof(uint32_t));
    pref_split = (uint32_t*)malloc((size_t)(MAX_LIMIT + 1U) * sizeof(uint32_t));

    uint32_t primes_cnt  = 0U;
    uint32_t splits_cnt  = 0U;

    pref_cnt[0U]   = 0U;
    pref_split[0U] = 0U;
    pref_cnt[1U]   = 0U;
    pref_split[1U] = 0U;

    for (uint32_t i = 2U; i <= MAX_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* i is prime */
            ++primes_cnt;
            if (is_split_prime(i)) ++splits_cnt;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i) {
                    is_comp[j] = 1U;
                }
            }
        }
        pref_cnt[i]   = primes_cnt;
        pref_split[i] = splits_cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Count the number of norms ≤ m                                      */
/* ------------------------------------------------------------------ */
static uint64_t count_leq(uint32_t m) {
    if (m > MAX_LIMIT) m = MAX_LIMIT;
    uint32_t split_val   = pref_split[m];
    uint32_t root        = isqrt32(m);
    uint32_t primes_root = pref_cnt[root];
    uint32_t split_root  = pref_split[root];
    uint32_t inert_root  = primes_root - split_root;   /* inert primes ≤ √m */

    return (uint64_t)split_val + (uint64_t)inert_root;
}

/* ------------------------------------------------------------------ */
/* Binary search for the nth norm                                    */
/* ------------------------------------------------------------------ */
static uint32_t find_nth_value(idx_t n) {
    uint32_t low  = 2U;                  /* smallest possible norm */
    uint32_t high = MAX_LIMIT;           /* ensure high is large enough */

    /* enlarge high if necessary (very unlikely for n ≤ 10000) */
    while (count_leq(high) < (uint64_t)n) {
        high *= 2U;
        if (high > MAX_LIMIT) break;     /* in case we run out of precomputed data */
    }

    while (low < high) {
        uint32_t mid = low + (high - low) / 2U;
        if (count_leq(mid) >= (uint64_t)n) {
            high = mid;
        } else {
            low = mid + 1U;
        }
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Public function requested by the framework                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* initialise only once */
    init_prime_data();

    if (n == 0) return (seq_t)0;          /* behaviour for offset < 1 is irrelevant */

    uint32_t val = find_nth_value(n);     /* value fits in 32‑bit for all n proved */
    return (seq_t)val;
}
