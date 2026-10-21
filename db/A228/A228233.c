/*
 * A228233  Number of Gaussian primes of norm less than or equal to n in the
 *          first quadrant.
 * https://oeis.org/A228233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*π₁(n²) + 2*π₃(n)
 * Range:     u8 n <= 40, u16 n <= 906, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where π₁(m) = #primes ≤ m with p ≡ 1 (mod 4),  */
/*        π₃(m) = #primes ≤ m with p ≡ 3 (mod 4)    */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Global data for cached primes */
static idx_t   current_limit = 0;          /* maximum prime limit built */
static uint32_t *primes1 = NULL;          /* primes ≡ 1 (mod 4) */
static size_t   primes1_cnt = 0;
static uint32_t *primes3 = NULL;          /* primes ≡ 3 (mod 4) */
static size_t   primes3_cnt = 0;

/* Rebuild prime lists up to a new limit */
static void rebuild_primes_upto(idx_t limit)
{
    if (limit <= current_limit) return;

    uint64_t *all_primes = NULL;
    size_t    all_cnt    = 0;
    build_primes_upto(limit, &all_primes, &all_cnt);

    /* Count primes in each residue class (skip 2) */
    size_t cnt1 = 0, cnt3 = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint64_t p = all_primes[i];
        if (p <= 2ULL) continue;
        uint32_t r = (uint32_t)(p & 3ULL);   /* p % 4 */
        if (r == 1) ++cnt1;
        else if (r == 3) ++cnt3;
    }

    /* Allocate new arrays */
    uint32_t *new1 = NULL, *new3 = NULL;
    if (cnt1 > 0) new1 = (uint32_t*)malloc(cnt1 * sizeof(uint32_t));
    if (cnt3 > 0) new3 = (uint32_t*)malloc(cnt3 * sizeof(uint32_t));

    /* Fill them */
    size_t i1 = 0, i3 = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint64_t p = all_primes[i];
        if (p <= 2ULL) continue;
        uint32_t r = (uint32_t)(p & 3ULL);
        if (r == 1) new1[i1++] = (uint32_t)p;
        else if (r == 3) new3[i3++] = (uint32_t)p;
    }

    /* Release old data */
    free(primes1);
    free(primes3);
    primes1 = new1;
    primes3 = new3;
    primes1_cnt = cnt1;
    primes3_cnt = cnt3;
    current_limit = limit;

    /* Free the temporary array of all primes */
    free(all_primes);
}

/* Binary search upper bound: number of elements <= val */
static size_t upper_bound_uint32(const uint32_t *arr, size_t n, idx_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)arr[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ----------------------------------------------------------------------- */
/* Public function required by the build system                            */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset starts at 1 */

    idx_t n2 = n * n;              /* n² fits in idx_t for n ≤ 10⁴ */

    /* Ensure the prime data covers n² (for π₁) and n (for π₃) */
    rebuild_primes_upto(n2);

    /* Count primes ≡ 1 (mod 4) up to n² */
    size_t cnt1 = upper_bound_uint32(primes1, primes1_cnt, n2);

    /* Count primes ≡ 3 (mod 4) up to n */
    size_t cnt3 = upper_bound_uint32(primes3, primes3_cnt, n);

    /* a(n) = 1 + 2*cnt1 + 2*cnt3 */
    return (seq_t)(1 + 2 * (seq_t)cnt1 + 2 * (seq_t)cnt3);
}
