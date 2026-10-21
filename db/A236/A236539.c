/*
 * A236539  Integer areas of integer-sided triangles such that the length of
 *          two sides are Fibonacci numbers.
 * https://oeis.org/A236539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate all integer‑area triangles whose two sides are
 * Fibonacci numbers.  Collect the distinct areas, sort them and
 * answer the n‑th (1‑based) term.  The data are computed once and
 * memoized between calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for unsigned 128‑bit values.  Works on
 * compilers supporting the built‑in __uint128_t type. */
static uint64_t isqrt128(__uint128_t x)
{
    uint64_t low = 0, high = (uint64_t)-1;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Dynamic array of discovered areas.  The array is sorted and unique
 * after compute_areas(). */
static seq_t *areas = NULL;
static size_t area_cnt = 0;
static size_t area_cap = 0;

/* Append a new area, expanding the backing array if needed. */
static void push_area(seq_t val)
{
    if (area_cnt == area_cap) {
        area_cap = area_cap ? area_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(areas, area_cap * sizeof(seq_t));
        if (!tmp) abort();
        areas = tmp;
    }
    areas[area_cnt++] = val;
}

/* Comparator for qsort that works for all seq_t types. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Compute all triangle areas up to a Fibonacci limit that is
 * sufficient to cover the first ~10 000 terms.  The function is
 * idempotent: it only runs once per process. */
static void compute_areas(void)
{
    if (areas != NULL) return;            /* already computed */

    /* Fibonacci limit chosen experimentally to generate >10 000
     * distinct areas while keeping the number of loops modest. */
    const uint64_t F_LIMIT = 500000;

    /* Generate Fibonacci numbers up to F_LIMIT. */
    uint64_t fib[32];
    size_t fibcnt = 0;
    fib[fibcnt++] = 1;
    fib[fibcnt++] = 1;
    while (1) {
        uint64_t nxt = fib[fibcnt - 1] + fib[fibcnt - 2];
        if (nxt > F_LIMIT) break;
        fib[fibcnt++] = nxt;
    }

    /* Enumerate all triangles with two Fibonacci sides (a <= b). */
    for (size_t i = 0; i < fibcnt; ++i) {
        uint64_t a = fib[i];
        for (size_t j = i; j < fibcnt; ++j) {
            uint64_t b = fib[j];

            /* Triangle inequality: |a−b| < c < a+b.  The starting
             * value of c must have the same parity as a+b. */
            uint64_t minc = (a > b) ? a - b + 1 : b - a + 1;
            uint64_t maxc = a + b - 1;
            if (((a + b + minc) & 1) == 1) ++minc;   /* adjust parity */
            for (uint64_t c = minc; c <= maxc; c += 2) {
                /* Heron's formula: area^2 = s*(s−a)*(s−b)*(s−c),
                 * where s = (a+b+c)/2 is integer. */
                const __uint128_t s = (__uint128_t)(a + b + c) >> 1;
                const __uint128_t term1 = s;
                const __uint128_t term2 = s - a;
                const __uint128_t term3 = s - b;
                const __uint128_t term4 = s - c;
                const __uint128_t areaSq = term1 * term2 * term3 * term4;
                if (areaSq == 0) continue;          /* degenerate */

                /* Test whether areaSq is a perfect square. */
                const uint64_t sqrt_val = isqrt128(areaSq);
                if ((__uint128_t)sqrt_val * sqrt_val == areaSq) {
                    push_area((seq_t)sqrt_val);
                }
            }
        }
    }

    /* Sort, deduplicate, and finalize the array. */
    qsort(areas, area_cnt, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < area_cnt; ++i) {
        if (uniq == 0 || areas[i] != areas[uniq - 1]) {
            areas[uniq++] = areas[i];
        }
    }
    area_cnt = uniq;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (areas == NULL) compute_areas();
    if (n == 0 || n > area_cnt) return 0;   /* out of range: return 0 */
    return areas[n - 1];
}
