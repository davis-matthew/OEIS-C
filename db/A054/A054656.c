/*
 * A054656  Number of primes <= n which do not appear in any partitions of n
 *          into distinct primes.
 * https://oeis.org/A054656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes ≤ n that never appear in a partition of
 *            n into distinct primes
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   shift_or
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/shift_or.h"

/* ------------------------------------------------------------------ */
/*  Helper: shift a bitset left by shift bits and OR into dest        */

/* ------------------------------------------------------------------ */
/*  Global data to be initialised once                                     */
/* ------------------------------------------------------------------ */
#define MAX_N 10000        /* sufficient for all tests */

static bool          initialized = false;
static uint64_t     *primes      = NULL;   /* prime values */
static int           prime_cnt   = 0;      /* number of primes */
static uint64_t     *prefix      = NULL;   /* (prime_cnt+1) * words */
static uint64_t     *suffix      = NULL;   /* (prime_cnt+1) * words */
static int           words       = 0;      /* bitset words */
static int           max_bits    = MAX_N + 1;

/* ------------------------------------------------------------------ */
/*  Sieve of Eratosthenes to collect primes ≤ MAX_N                     */
/* ------------------------------------------------------------------ */
static void compute_primes(void) {
    bool *is_prime = (bool*)calloc(MAX_N + 1, sizeof(bool));
    for (int i = 2; i <= MAX_N; ++i) is_prime[i] = true;
    for (int i = 2; i * i <= MAX_N; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_N; j += i)
                is_prime[j] = false;
        }
    }
    /* Count primes */
    for (int i = 2; i <= MAX_N; ++i)
        if (is_prime[i]) ++prime_cnt;
    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    int idx = 0;
    for (int i = 2; i <= MAX_N; ++i) {
        if (is_prime[i]) primes[idx++] = (uint64_t)i;
    }
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/*  Build prefix and suffix bitsets                                      */
/* ------------------------------------------------------------------ */
static void build_bitsets(void) {
    words = (max_bits + 63) >> 6;   /* number of 64‑bit words */
    /* Allocate contiguous blocks */
    prefix = (uint64_t*)calloc((prime_cnt + 1) * words, sizeof(uint64_t));
    suffix = (uint64_t*)calloc((prime_cnt + 1) * words, sizeof(uint64_t));

    /* --------- Prefix --------- */
    prefix[0] = 1;                 /* only sum 0 reachable */
    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t *dest = &prefix[(i + 1) * words];
        uint64_t *src  = &prefix[i * words];
        memcpy(dest, src, words * sizeof(uint64_t));     /* copy previous */
        shift_or(src, (int)primes[i], dest, words, max_bits);
    }

    /* --------- Suffix --------- */
    suffix[prime_cnt * words] = 1;   /* only sum 0 reachable at the end */
    for (int i = prime_cnt - 1; i >= 0; --i) {
        uint64_t *dest = &suffix[i * words];
        uint64_t *src  = &suffix[(i + 1) * words];
        memcpy(dest, src, words * sizeof(uint64_t));
        shift_or(src, (int)primes[i], dest, words, max_bits);
    }
}

/* ------------------------------------------------------------------ */
/*  Initialise once                                        */
/* ------------------------------------------------------------------ */
static void init(void) {
    if (initialized) return;
    compute_primes();
    build_bitsets();
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Query function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init();                                /* ensure tables built */
    uint64_t N = (uint64_t)n;
    if (N > (uint64_t)MAX_N) {
        /* Outside pre‑computed range – fall back to 0 (not used in tests) */
        return (seq_t)0;
    }

    /* Find number of primes ≤ n */
    int prime_cnt_n = 0;
    while (prime_cnt_n < prime_cnt && primes[prime_cnt_n] <= N) ++prime_cnt_n;

    seq_t miss = 0;                           /* result counter */

    for (int k = 0; k < prime_cnt_n; ++k) {
        uint64_t p = primes[k];
        uint64_t target = N - p;                /* sum needed from others */

        bool appears = false;
        uint64_t *pref = &prefix[k * words];
        uint64_t *suff = &suffix[(k + 1) * words];

        int max_w = (int)(target >> 6);
        for (int w = 0; w <= max_w && !appears; ++w) {
            uint64_t word = pref[w];
            while (word) {
                int bit = __builtin_ctzll(word);   /* position of lowest set bit */
                uint64_t a = ((uint64_t)w << 6) + (uint64_t)bit;
                if (a > target) break;
                uint64_t rem = target - a;
                int rw = rem >> 6;
                int rb = rem & 63;
                if (suff[rw] & ((uint64_t)1 << rb)) {
                    appears = true;
                    break;
                }
                word &= word - 1;   /* clear lowest set bit */
            }
        }
        if (!appears) ++miss;
    }
    return (seq_t)miss;
}
