/*
 * A082175  Number of reduced indefinite quadratic forms over the integers in
 *          two variables with discriminants D(n)=A079896(n).
 * https://oeis.org/A082175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2710, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082175: number of reduced indefinite quadratic forms over the integers
   for discriminants D(n)=A079896(n).  The function counts all forms,
   including non‑primitive ones, by iterating over admissible values of
   b and enumerating factor pairs of |ac|. It uses a small cache for 
   the A079896 sequence and a simple integer square root routine. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned values. */
static inline idx_t
int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Test whether a 64‑bit integer is a perfect square. */
static inline bool
is_square(idx_t n)
{
    idx_t r = int_sqrt(n);
    return (__uint128_t)r * r == n;
}

/* Cache for the A079896 sequence: nth non‑square integer that is 0 or 1 mod 4. */
static idx_t  *D_cache      = NULL;
static size_t  D_cache_cnt  = 0;
static size_t  D_cache_cap  = 0;

static void
ensure_D_cache(idx_t n)
{
    if (n <= D_cache_cnt)
        return;

    if (!D_cache_cap)
        D_cache_cap = 16;
    while (D_cache_cap < n)
        D_cache_cap <<= 1;

    D_cache = (idx_t*)realloc(D_cache, D_cache_cap * sizeof(idx_t));

    idx_t next = D_cache_cnt ? D_cache[D_cache_cnt - 1] + 1 : 1;
    while (D_cache_cnt < n) {
        if (((next & 3ull) == 0 || (next & 3ull) == 1) && !is_square(next))
            D_cache[D_cache_cnt++] = next;
        ++next;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Contract: n >= 1 */

    ensure_D_cache(n);
    idx_t D = D_cache[n - 1];                 /* Discriminant */

    /* f(D) = ceil(sqrt(D)) */
    idx_t f = int_sqrt(D);
    if ((__uint128_t)f * f < D)
        ++f;

    seq_t count = 0;

    /* Iterate over admissible b: 1 <= b < f , b^2 ≡ D (mod 4). */
    for (idx_t b = 1; b < f; ++b) {
        int64_t diff = (int64_t)b * (int64_t)b - (int64_t)D; /* b^2 - D < 0 for b < sqrt(D) */
        if ((uint64_t)diff & 3ull)     /* (b^2 - D) must be ≡ 0 (mod 4) */
            continue;

        /* |ac| = (D - b^2)/4 = -diff/4 */
        uint64_t absN = (diff >= 0) ? (uint64_t)diff : (uint64_t)(-diff);
        absN >>= 2;  /* divide by 4 */

        /* Reduced condition: min(|a|,|c|) >= ceil((f - b)/2) */
        idx_t limit = (f - b + 1) >> 1;   /* ceil((f - b)/2) */

        idx_t lim = int_sqrt(absN);
        for (idx_t d = 1; d <= lim; ++d) {
            if (absN % d != 0)
                continue;
            idx_t e = absN / d;   /* e >= d */

            if (d < limit) /* ensures both |a| and |c| >= limit */
                continue;

            if (d == e)
                count += (seq_t)2;     /* (d,-d), (-d,d) */
            else
                count += (seq_t)4;     /* (d,-e),(e,-d),(-d,e),(-e,d) */
        }
    }
    return count;
}
