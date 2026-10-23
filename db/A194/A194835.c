/*
 * A194835  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-sqrt(2).
 * https://oeis.org/A194835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   analogous to sorting the indices 1..n by descending fractional
 *            parts of k*sqrt(2). The algorithm uses only integer arithmetic
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: integer square root of a 128‑bit unsigned integer */
static uint64_t isqrt128(unsigned __int128 v)
{
    uint64_t lo = 0, hi = (uint64_t) -1;   /* hi = 2^64‑1; square fits in 128 bits */
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* helper: floor of k*sqrt(2)  (exact).  Uses floor(sqrt(2*k^2)). */
static uint64_t floor_k_sqrt2(uint64_t k)
{
    unsigned __int128 val = (unsigned __int128)2 * k * k;
    return isqrt128(val);
}

/* helper: find minimal R with R*(R+1)/2 >= n */
static uint64_t find_row(uint64_t n)
{
    uint64_t low = 1, high = 1;
    while ( ((unsigned __int128)high * (high + 1)) / 2 < n )
        high <<= 1;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* global array of floor values for current call, used in the comparator */
static const uint64_t *g_floor_arr = NULL;
static size_t g_floor_count = 0;

/* comparator: descending order of fractional parts {k*sqrt(2)} */
static int cmp_desc(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    if (a == b) return 0;

    uint64_t lo = a < b ? a : b;
    uint64_t hi = a < b ? b : a;

    uint64_t d_floor = g_floor_arr[hi - 1] - g_floor_arr[lo - 1];
    uint64_t d_idx   = hi - lo;

    unsigned __int128 left  = (unsigned __int128)2 * d_idx * d_idx;
    unsigned __int128 right = (unsigned __int128)d_floor * d_floor;

    /* if left < right -> floor[lo] > floor[hi] in fractional part */
    if (left < right) {          /* lo has larger fractional part */
        return (lo == a) ? -1 : 1;
    } else {                     /* hi has larger fractional part */
        return (hi == a) ? -1 : 1;
    }
}

/* public function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0) return (seq_t)0; /* undefined but return 0 for safety */

    /* determine the row containing N */
    uint64_t R = find_row(N);

    /* position within the row (0‑based) */
    uint64_t prev_sum = ((unsigned __int128)(R - 1) * R) / 2;
    uint64_t pos = N - prev_sum - 1;

    /* compute floor values floor(k*sqrt(2)) for k = 1..R */
    uint64_t *floors = (uint64_t *)malloc(R * sizeof(uint64_t));
    if (!floors) return (seq_t)0;   /* allocation failure */

    for (uint64_t k = 1; k <= R; ++k)
        floors[k - 1] = floor_k_sqrt2(k);

    g_floor_arr   = floors;
    g_floor_count = R;

    /* build array of indices 1..R and sort */
    uint64_t *indices = (uint64_t *)malloc(R * sizeof(uint64_t));
    if (!indices) {
        free(floors);
        return (seq_t)0;
    }
    for (uint64_t i = 0; i < R; ++i) indices[i] = i + 1;

    qsort(indices, R, sizeof(uint64_t), cmp_desc);

    /* fetch the answer and clean up */
    uint64_t answer = indices[pos];
    free(indices);
    free(floors);
    return (seq_t)answer;
}
