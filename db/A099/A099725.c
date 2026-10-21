/*
 * A099725  a(n) is the number of 1's in the period of the continued fraction
 *          of the square root of the n-th nonsquare integer.
 * https://oeis.org/A099725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1’s in the period of the continued fraction of
 *            √(D), where D is the n‑th non‑square integer (D = 2, 3, 5, 6, 7,
 *            8, 10 …)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Forward declaration of the function required by the framework */
seq_t a(idx_t n);

/* ---------- Standard library headers ---------- */
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Helper: integer square root (uint64_t) ---------- */
static inline uint64_t isqrt_ull(uint64_t v)
{
    uint64_t lo = 0, hi = v;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* ---------- Helper: square test for uint64_t ---------- */
static inline bool is_square_uint64(uint64_t v)
{
    uint64_t r = isqrt_ull(v);
    return r * r == v;
}

/* ---------- Storage for non‑square integers ---------- */
static uint64_t *non_squares = NULL;          /* array of non‑squares */
static size_t non_squares_size = 0;           /* number stored */
static size_t non_squares_capacity = 0;       /* allocated capacity */
static uint64_t non_square_count = 0;         /* how many non‑squares known */
static uint64_t current_candidate = 2;        /* next integer to test */

/* Ensure we have at least 'need' non‑square integers stored. */
static void ensure_non_squares(size_t need)
{
    if (non_square_count >= need) return;

    if (non_squares_capacity == 0) {
        non_squares_capacity = 32;
        non_squares = (uint64_t *)malloc(non_squares_capacity * sizeof(uint64_t));
    } else if (non_square_count + (need - non_square_count) > non_squares_capacity) {
        while (non_squares_capacity < non_square_count + (need - non_square_count))
            non_squares_capacity <<= 1;
        non_squares = (uint64_t *)realloc(non_squares, non_squares_capacity * sizeof(uint64_t));
    }

    while (non_square_count < need) {
        if (!is_square_uint64(current_candidate)) {
            non_squares[non_square_count++] = current_candidate;
        }
        current_candidate++;
    }
}

/* ---------- Period computation for sqrt(D) ---------- */
static inline seq_t count_ones_in_period(uint64_t D)
{
    uint64_t a0 = isqrt_ull(D);
    uint64_t m = 0, d = 1, a = a0;
    seq_t ones = 0;

    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (a == 1) ones++;

        if (a == 2 * a0) break;    /* end of period */
    }

    return ones;
}

/* ---------- Storage for sequence a(n) ---------- */
static seq_t *cache = NULL;              /* array of computed values */
static size_t cache_size = 0;             /* how many stored (valid indices 0..size-1) */
static size_t cache_capacity = 0;         /* allocated capacity */

/* Ensure cache has space for at least 'size' elements */
static void ensure_cache_capacity(size_t size)
{
    if (cache_capacity >= size) return;

    if (cache_capacity == 0) {
        cache_capacity = 32;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    } else {
        while (cache_capacity < size) cache_capacity <<= 1;
    }
    cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
}

/* ---------- The required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 in any case */

    size_t need = (size_t)n;

    /* Make sure we have the n‑th non‑square integer */
    ensure_non_squares(need);

    /* Make sure we have computed a(1)…a(n) */
    if (cache_size < need) {
        ensure_cache_capacity(need);
        for (size_t i = cache_size; i < need; ++i) {
            uint64_t D = non_squares[i];                /* 0‑based index */
            cache[i] = count_ones_in_period(D);
        }
        cache_size = need;
    }

    return cache[need - 1];
}
