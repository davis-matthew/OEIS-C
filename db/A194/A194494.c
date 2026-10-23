/*
 * A194494  Number of ways to arrange 4 nonattacking queens on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 3*a(n-3) -5*a(n-4) -11*a(n-5) -3*a(n-6) *
 *            +11*a(n-7) +14*a(n-8) +6*a(n-9) -7*a(n-10) -16*a(n-11) *
 *            -14*a(n-12) +14*a(n-14) +16*a(n-15) +7*a(n-16) -6*a(n-17) *
 *            -14*a(n-18) -11*a(n-19) +3*a(n-20) +11*a(n-21) +5*a(n-22) *
 *            -3*a(n-23) -4*a(n-24) +a(n-26) * * Initial values: a(1)=0, a...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 36, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   compute_count, count_for_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_count.h"
#include "../utils/count_for_rows.h"

/* Forward declarations for helper functions used below. */
static uint64_t count_for_rows(uint64_t r1, uint64_t r2, uint64_t r3,
                               uint64_t r4, uint64_t n);
static uint64_t compute_count(uint64_t n);

/* Static storage for the computed sequence. */
static seq_t *seq_cache   = NULL;  /* array indexed by n (n≥0) */
static idx_t  cache_len   = 0;     /* largest n that has a computed value */
static idx_t  cache_cap   = 0;     /* total allocated capacity (≥ cache_len+1) */

/* Ensure that seq_cache has room for values up to 'need' (inclusive). */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failure – abort.  The build environment never
         * expects such a failure; an explicit abort makes the
         * problem obvious.  */
        abort();
    }
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Initialise the cache with the known initial terms and by
 * computing a(25) and a(26) if necessary. */
static void init_cache(void)
{
    if (cache_len > 0)
        return;   /* already initialised */

    /* Reserve space for all indexes up to 26.  Larger requests
     * will grow the array as needed. */
    ensure_cache_capacity(27);

    /* Index 0 is unused; set it to a harmless value. */
    seq_cache[0] = 0;

    /* Known initial values for n = 1 … 24 (taken from the OEIS entry). */
    static const uint64_t first[25] = {
        0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 8ULL, 118ULL, 802ULL, 3708ULL,
        13280ULL, 39734ULL, 104000ULL, 244948ULL, 530632ULL, 1072776ULL,
        2048056ULL, 3723314ULL, 6492342ULL, 10915254ULL, 17777372ULL,
        28147380ULL, 43465356ULL, 65624634ULL, 97098802ULL
    };
    for (idx_t i = 1; i <= 24; ++i)
        seq_cache[i] = (seq_t)first[i];

    /* Compute a(25) and a(26) via exhaustive search. */
    seq_cache[25] = (seq_t)compute_count(25);
    seq_cache[26] = (seq_t)compute_count(26);

    cache_len = 26;
}

/* Public sequence function.  Handles calls in any order and
 * efficiently extends the cache when required. */
seq_t a(idx_t n)
{
    init_cache();

    /* Grow the array if the caller requests a value beyond the
     * current capacity. */
    if ((idx_t)(n + 1) > cache_cap)
        ensure_cache_capacity(n + 1);

    if (n <= cache_len)
        return seq_cache[n];

    /* Compute values up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t res = 0;
        res += 4 * seq_cache[i - 2];
        res += 3 * seq_cache[i - 3];
        res -= 5 * seq_cache[i - 4];
        res -= 11 * seq_cache[i - 5];
        res -= 3 * seq_cache[i - 6];
        res += 11 * seq_cache[i - 7];
        res += 14 * seq_cache[i - 8];
        res += 6 * seq_cache[i - 9];
        res -= 7 * seq_cache[i - 10];
        res -= 16 * seq_cache[i - 11];
        res -= 14 * seq_cache[i - 12];
        res += 14 * seq_cache[i - 14];
        res += 16 * seq_cache[i - 15];
        res += 7 * seq_cache[i - 16];
        res -= 6 * seq_cache[i - 17];
        res -= 14 * seq_cache[i - 18];
        res -= 11 * seq_cache[i - 19];
        res += 3 * seq_cache[i - 20];
        res += 11 * seq_cache[i - 21];
        res += 5 * seq_cache[i - 22];
        res -= 3 * seq_cache[i - 23];
        res -= 4 * seq_cache[i - 24];
        res += seq_cache[i - 26];
        seq_cache[i] = res;
    }
    cache_len = n;
    return seq_cache[n];
}
