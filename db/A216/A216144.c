/*
 * A216144  Square root of smallest square greater than the product of first n
 *          primes.
 * https://oeis.org/A216144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor_sqrt(∏_{i=1..n} p_i) + 1 where p_i is the i-th
 *            prime. The sequence is OEIS A216144
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto, ensure_cache, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/ensure_cache.h" /* ensure_cache(idx_t, seq_t **, size_t *) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t **, size_t *) */


// ---------- 128‑bit integer helpers (available on GCC/Clang) ----------
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    /* Binary search for floor(sqrt(x)) in 0 … 2^64-1. */
    unsigned __int128 low  = 0;
    unsigned __int128 high = (((unsigned __int128)1) << 64) - 1U;
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}
// ---------------------------------------------------------------


/* Cached data ---------------------------------------------------- */
/* Cache of already computed a(i) (1‑based).  Values are stored
   0‑based inside the array.  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
/* How many elements have been computed so far. */
static idx_t cur_n = 0;

/* Cached product of the first cur_n primes.  Two representations
   are used:  for seq_t that fits in 8 bytes or less we keep the
   product in a 128‑bit unsigned integer;  otherwise we use
   seq_t itself.  */
static unsigned __int128 prod128 = 1U;
static seq_t         prod_seq  = 0U;

/* Array of the first primes that have been generated.  */
static uint64_t *prime_arr = NULL;
static size_t    prime_cnt = 0;
/* ----------------------------------------------------------------- */

/* Helper: initializes the prime list up to a fixed limit.
   The limit is sufficiently large for all requested indices (≤10000). */
static void init_primes(void)
{
    if (prime_arr != NULL)
        return;
    /* 200 000 is > the 10 000‑th prime (104 729). */
    const idx_t LIM = 200000;
    build_primes_upto(LIM, &prime_arr, &prime_cnt);
}

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Zero or negative indices are undefined; return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the prime list is built. */
    init_primes();

    /* Ensure the cache can hold n elements. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the product representation if this is the first call. */
    if (cur_n == 0) {
        if (sizeof(seq_t) <= 8) {
            prod128 = 1U;   /* 128‑bit product starts at 1 */
        } else {
            prod_seq = 1U;  /* seq_t product starts at 1 */
        }
    }

    /* Generate values up to n, extending product and cache. */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        uint64_t p = prime_arr[(size_t)(i - 1)];

        /* Update product. */
        if (sizeof(seq_t) <= 8) {
            prod128 *= (unsigned __int128)p;
        } else {
            prod_seq *= (seq_t)p;
        }

        /* Compute floor(sqrt(product)) and add 1. */
        seq_t val;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 sq = isqrt128(prod128);
            val = (seq_t)sq + 1U;
        } else {
            seq_t sq = floor_sqrt(prod_seq);
            val = sq + 1U;
        }

        /* Store into cache. */
        cache[(size_t)(i - 1)] = val;
    }

    cur_n = n;
    return cache[(size_t)(n - 1)];
}
