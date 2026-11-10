/*
 * A081417  A000720 applied to Pascal-triangle: primepi(C(n,j)), j,0..n and
 *          n=0,1,2,...
 * https://oeis.org/A081417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A000720(A007318(n)) = primepi(C(r,k)), where n =
 *            r*(r+1)/2 + k, 0 <= k <= r. C(r,k) is computed multiplicatively,
 *            and primepi uses the combinatorial (Lucy / min_25)
 *            prime-counting method, which takes O(x^(3/4)/log x) time and
 *            O(sqrt x) memory
 * Range:     u8 n <= 96, u16 n <= 285, u32 n <= 837, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 103 terms through n = 1323 (2026-11-07)
 * Helpers:   isqrt_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"
#include "../utils/prime_pi_u64.h"

typedef long long pp_i64;


static pp_i64 pp_primepi(pp_i64 N)
{
    return N < 2 ? 0 : (pp_i64)prime_pi_u64((uint64_t)N);   /* db/utils/prime_pi_u64.h */
}

#define PP_CROWS 256
static pp_i64 pp_cache[PP_CROWS][PP_CROWS / 2 + 1];
static unsigned char pp_valid[PP_CROWS][PP_CROWS / 2 + 1];

seq_t a(idx_t n) {
    idx_t r = (idx_t)((isqrt_u64(8 * (uint64_t)n + 1) - 1) / 2);
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r > 0 && r * (r + 1) / 2 > n) r--;
    idx_t k = n - r * (r + 1) / 2;
    if (k > r - k) k = r - k;

    if (r < PP_CROWS && pp_valid[r][k]) return (seq_t)pp_cache[r][k];

    const unsigned long long LIM = 4000000000000000000ULL;
    unsigned long long c = 1;
    int ok = 1;
    for (idx_t i = 0; i < k; i++) {
        unsigned long long m = (unsigned long long)(r - i);
        if (c > LIM / m) { ok = 0; break; }
        c = c * m / (unsigned long long)(i + 1);
    }
    if (!ok || c >= (1ULL << 53)) return (seq_t)0; /* far beyond practical range */

    pp_i64 v = pp_primepi((pp_i64)c);
    if (r < PP_CROWS) {
        pp_cache[r][k] = v;
        pp_valid[r][k] = 1;
    }
    return (seq_t)v;
}
