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
 * Validated: standard, pass, 46 terms through n = 78 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute a(n) for 1 ≤ n ≤ 26 by exhaustive search. */
static uint64_t count_for_rows(uint64_t r1, uint64_t r2, uint64_t r3,
                               uint64_t r4, uint64_t n)
{
    const uint64_t rows[4] = { r1, r2, r3, r4 };
    uint64_t cnt = 0;

    /* Iterate over all legal column choices for each of the four rows. */
    for (uint64_t c1 = 1; c1 <= rows[0]; ++c1) {
        uint64_t col1   = 1ULL << (c1 - 1);
        uint64_t d11    = (rows[0] - c1) + (n - 1);
        uint64_t diag1  = 1ULL << d11;
        uint64_t ad1    = rows[0] + c1;
        uint64_t adiag1 = 1ULL << (ad1 - 2);

        for (uint64_t c2 = 1; c2 <= rows[1]; ++c2) {
            uint64_t col2   = 1ULL << (c2 - 1);
            if (col1 & col2)
                continue;
            uint64_t d12    = (rows[1] - c2) + (n - 1);
            if (diag1 & (1ULL << d12))
                continue;
            uint64_t ad2    = rows[1] + c2;
            if (adiag1 & (1ULL << (ad2 - 2)))
                continue;
            uint64_t col12   = col1 | col2;
            uint64_t diag12  = diag1 | (1ULL << d12);
            uint64_t adiag12 = adiag1 | (1ULL << (ad2 - 2));

            for (uint64_t c3 = 1; c3 <= rows[2]; ++c3) {
                uint64_t col3   = 1ULL << (c3 - 1);
                if (col12 & col3)
                    continue;
                uint64_t d13    = (rows[2] - c3) + (n - 1);
                if (diag12 & (1ULL << d13))
                    continue;
                uint64_t ad3    = rows[2] + c3;
                if (adiag12 & (1ULL << (ad3 - 2)))
                    continue;
                uint64_t col13   = col12 | col3;
                uint64_t diag13  = diag12 | (1ULL << d13);
                uint64_t adiag13 = adiag12 | (1ULL << (ad3 - 2));

                for (uint64_t c4 = 1; c4 <= rows[3]; ++c4) {
                    uint64_t col4   = 1ULL << (c4 - 1);
                    if (col13 & col4)
                        continue;
                    uint64_t d14    = (rows[3] - c4) + (n - 1);
                    if (diag13 & (1ULL << d14))
                        continue;
                    uint64_t ad4    = rows[3] + c4;
                    if (adiag13 & (1ULL << (ad4 - 2)))
                        continue;
                    /* All four queens are non‑attacking. */
                    ++cnt;
                }
            }
        }
    }
    return cnt;
}

/* Brute‑force enumeration of all 4‑queen arrangements on the lower
 * triangle of an n × n board.  Only called for n ≤ 26. */
static uint64_t compute_count(uint64_t n)
{
    uint64_t total = 0;
    for (uint64_t r1 = 1; r1 <= n - 3; ++r1)
        for (uint64_t r2 = r1 + 1; r2 <= n - 2; ++r2)
            for (uint64_t r3 = r2 + 1; r3 <= n - 1; ++r3)
                for (uint64_t r4 = r3 + 1; r4 <= n; ++r4)
                    total += count_for_rows(r1, r2, r3, r4, n);
    return total;
}

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
    seq_t *new_arr = mem_resize(seq_t, seq_cache, new_cap);
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
