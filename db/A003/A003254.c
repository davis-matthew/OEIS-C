/*
 * A003254  The number m such that A003233(m) = A005206(A003234(n)).
 * https://oeis.org/A003254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G(t) - #{j : A003234(j) <= G(t)}, where t = A003234(n),
 *            G(x) = A005206(x) = floor((x+1)/phi). A003233 and A003234 split
 *            the positive integers: k is in A003234 iff U(k+U(k)) - U(k) -
 *            U(U(k)) = 1, with U = A001950 (upper Wythoff) and A003231(k) = k
 *            + U(k). The terms of A003234 are found by s...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isqrt_u64.h"

/* floor(k*phi) */
static uint64_t a3254_L(uint64_t k)
{
    return (k + isqrt_u64(5 * k * k)) / 2;
}

/* floor(k*phi^2) = A001950(k) */
static uint64_t a3254_U(uint64_t k)
{
    return k + a3254_L(k);
}

static int a3254_in234(uint64_t k)
{
    uint64_t u = a3254_U(k);
    return a3254_U(k + u) - u - a3254_U(u) == 1;
}

static uint64_t *a3254_T = NULL;
static size_t a3254_cnt = 0, a3254_cap = 0;
static uint64_t a3254_next = 1;

static void a3254_ensure(size_t n)
{
    while (a3254_cnt < n) {
        uint64_t k = a3254_next++;
        if (a3254_in234(k)) {
            if (a3254_cnt == a3254_cap) {
                size_t nc = a3254_cap ? a3254_cap * 2 : 1024;
                a3254_T = (uint64_t *)realloc(a3254_T, nc * sizeof(uint64_t));
                a3254_cap = nc;
            }
            a3254_T[a3254_cnt++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t t, g, lo, hi;
    if (n < 1) return (seq_t)0;
    a3254_ensure((size_t)n);
    t = a3254_T[n - 1];
    g = a3254_L(t + 1) - (t + 1); /* floor((t+1)/phi) */
    /* count of A003234 terms <= g */
    lo = 0;
    hi = (uint64_t)n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a3254_T[mid] <= g) lo = mid + 1;
        else hi = mid;
    }
    return (seq_t)(g - lo);
}
