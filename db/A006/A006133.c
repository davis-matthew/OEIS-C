/*
 * A006133  Related to representations as sums of Fibonacci numbers.
 * https://oeis.org/A006133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003231(x) = A003234(x) for x = A006132(n); computed by
 *            search: A003231(x) = floor(x*(5+sqrt(5))/2) = A000201(x) + 2x,
 *            and A003234 lists the m with A003231(A001950(m)) =
 *            A001950(A003231(m)) - 1; walk A003234 in order and keep the
 *            values where its j-th term equals A003231(j)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/isqrt_u64.h"

/* floor(n*phi) */
static uint64_t a6133_lo(uint64_t n) { return (n + isqrt_u64(5 * n * n)) / 2; }

/* floor(n*(phi+2)) = A003231 */
static uint64_t a6133_c(uint64_t n) { return a6133_lo(n) + 2 * n; }

/* floor(n*phi^2) = A001950 */
static uint64_t a6133_hi(uint64_t n) { return a6133_lo(n) + n; }

static int a6133_in3234(uint64_t m)
{
    return a6133_c(a6133_hi(m)) + 1 == a6133_hi(a6133_c(m));
}

static uint64_t *a6133_res = NULL;
static idx_t a6133_cnt = 0, a6133_cap = 0;
static uint64_t a6133_m = 0, a6133_j = 0;

seq_t a(idx_t n)
{
    while (a6133_cnt < n) {
        a6133_m++;
        if (a6133_in3234(a6133_m)) {
            a6133_j++;
            if (a6133_c(a6133_j) == a6133_m) {
                if (a6133_cnt == a6133_cap) {
                    idx_t nc = a6133_cap ? 2 * a6133_cap : 1024;
                    a6133_res = (uint64_t *)realloc(a6133_res, nc * sizeof(uint64_t));
                    a6133_cap = nc;
                }
                a6133_res[a6133_cnt++] = a6133_m;
            }
        }
    }
    return (seq_t)a6133_res[n - 1];
}
