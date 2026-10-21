/*
 * A050796  Numbers n such that n^2 + 1 is expressible as the sum of two
 *          nonzero squares in at least one way (the trivial solution n^2 + 1
 *          = n^2 + 1^2 is not counted).
 * https://oeis.org/A050796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computes A050796: numbers n such that n^2+1 has a nontrivial sum of
   two squares representation. The function builds the sequence on demand
   by testing each integer n for an additional representation
   (excluding the trivial n^2+1 = n^2+1^2). */

/* ---------- helper: integer square root for uint64_t ---------- */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ---------- check whether n satisfies the definition ---------- */
static bool is_good(idx_t k)
{
    if (k == 1)                /* first term is always included */
        return true;

    uint64_t m = (uint64_t)k * k + 1;          /* m = n^2 + 1   */
    uint64_t limit = isqrt_uint64(m / 2);      /* a <= b, so a^2 <= m/2 */

    for (uint64_t a = 2; a <= limit; ++a) {    /* start from 2 to skip trivial (1,n) */
        uint64_t a2 = a * a;
        uint64_t b2 = m - a2;
        uint64_t b  = isqrt_uint64(b2);
        if (b * b == b2 && b >= a)            /* found nontrivial pair (a,b) */
            return true;
    }
    return false;
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache = NULL;      /* array of found terms */
static size_t seq_len   = 0;         /* number of stored terms   */
static size_t seq_cap   = 0;         /* allocated capacity       */
static idx_t  next_k    = 1;         /* next integer to test    */

/* ensure capacity for at least 'needed' elements */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < needed)
            new_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ------------------------------------- public function ------------------------------------- */
seq_t a(idx_t n)
{
    while (seq_len < n) {            /* generate terms until we have n of them */
        if (is_good(next_k)) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
