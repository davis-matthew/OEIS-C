/*
 * A143343  Triangle T(n,k) (n>=0, 1<=k<=n+1) read by rows: T(n,1)=1 for n>=0,
 *          T(1,2)=2. If n>=3 is odd then T(n,k)=1 for all k. If n>=3 is even
 *          then if k is prime and k-1 divides n then T(n,k)=k, otherwise
 *          T(n,k)=1.
 * https://oeis.org/A143343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements triangle A143343:
   T(n,1)=1 for all n>=0; T(1,2)=2.
   If n>=3 is odd then T(n,k)=1 for all k.
   If n>=3 is even then T(n,k)=k  if k is prime and k−1 | n,
   otherwise T(n,k)=1.
   The linear index n is the position in the flattened triangle
   (rows of lengths 1,2,3,…).  The function returns the value of
   a(n) at that index. */

#include <stdint.h>
#include <stdbool.h>

/* ----- Small integer square root for __uint128_t values ----- */
static uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* hi^2 < x < (hi+1)^2 */
}

/* ----- Prime test for 64‑bit values (trial division) ----- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; (__uint128_t)d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* ----- Offset of the first element of row r: r*(r+1)/2 (128‑bit) ----- */
static uint64_t offset_row(uint64_t r)
{
    __uint128_t tmp = (__uint128_t)r * (r + 1ULL) / 2ULL;
    return (uint64_t)tmp;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    __uint128_t tmp = (__uint128_t)8 * n + 1;
    uint64_t sq = isqrt_u128(tmp);
    uint64_t r  = (sq - 1ULL) >> 1;   /* floor((√(8n+1)-1)/2) */

    uint64_t off = offset_row(r);
    uint64_t k   = n - off + 1;       /* column index (1‑based) */

    /* Evaluate triangle entry */
    if (k == 1ULL)                     /* first column */
        return (seq_t)1;
    if (r == 1ULL && k == 2ULL)        /* special entry T(1,2) */
        return (seq_t)2;
    if (r == 2ULL)                     /* row 2: T(2,k)=k for all k */
        return (seq_t)k;
    if (r >= 3ULL) {
        if (r & 1ULL)                  /* odd row (>=3) */
            return (seq_t)1;
        else                            /* even row (>=3) */
            if (is_prime_uint64(k) && ((r % (k - 1ULL)) == 0ULL))
                return (seq_t)k;
    }
    return (seq_t)1;
}
