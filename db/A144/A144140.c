/*
 * A144140  Numbers n such that between n^K and (n+1)^K there are no primes,
 *          where K = 3/2.
 * https://oeis.org/A144140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A144140: the n‑th integer n such
   that no prime lies strictly between n^(3/2) and (n+1)^(3/2). */
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system. */
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static inline bool is_square_uint64(idx_t x)
{
    seq_t sx = (seq_t)x;
    seq_t r  = floor_sqrt(sx);
    return r * r == sx;
}

static inline bool qualifies(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* n^(3/2) = sqrt(n^3) */
    seq_t n3   = nseq * nseq * nseq;
    seq_t low  = floor_sqrt(n3) + (seq_t)1;   /* first integer > n^(3/2) */

    /* (n+1)^(3/2) */
    idx_t n1   = n + (idx_t)1;
    seq_t n1seq = (seq_t)n1;
    seq_t n13  = n1seq * n1seq * n1seq;
    seq_t high = floor_sqrt(n13);             /* floor((n+1)^(3/2)) */

    /* If (n+1)^(3/2) is integer, exclude that endpoint. */
    if (is_square_uint64(n1)) {
        if (high > (seq_t)0) high -= (seq_t)1;
    }

    if (low > high) return true;              /* empty interval */

    /* Scan the interval for a prime. */
    for (seq_t p = low; p <= high; ++p) {
        if (is_prime_big(p)) return false;    /* a prime was found */
    }
    return true;                              /* no primes in the interval */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;       /* undefined for offset 0 */

    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here. */
    return (seq_t)candidate;
}
