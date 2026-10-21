/*
 * A096115  If n = (2^k)-1, a(n) = a((n+1)/2) = k, if n = 2^k, a(n) = a(n-1)+1
 *          = k+1, otherwise a(n) = (A000523(n)+1)*a(A035327(n-1)).
 * https://oeis.org/A096115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 * a(1) = 1
 * If n is a power of 2:          a(n) = a(n‑1) + 1
 * If n = 2^k – 1:                a(n) = k
 * Otherwise:                    a(n) = (⌊log₂(n)⌋ + 1) * a( 2^⌊log₂(n)⌋ – (n – 2^⌊log₂(n)⌋) )
 * (this is equivalent to the recurrence written in the OEIS entry).
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t */
#include <stdlib.h>   /* abort */

/* choose a cache size that comfortably covers all n that will be requested
   (< 10 000 in typical tests).  The array is static so it works for both
   C (seq_t = unsigned integer type) and C++ (seq_t = big‑int class). */
#ifndef CACHE_MAX
#  define CACHE_MAX 65536
#endif

/* static cache:  cache[i] holds a(i) if known[i] is 1 */
static seq_t   cache[CACHE_MAX];
static uint8_t known[CACHE_MAX];

/* recursively compute a(n), using the cache when possible */
static seq_t eval_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* fall back to direct recursion if the index exceeds the
       preallocated cache (should not happen for usual inputs) */
    if (n >= CACHE_MAX) {
        idx_t p = 1;
        idx_t m = 0;
        while (p <= n >> 1) {
            p <<= 1;
            ++m;
        }
        if (p == n)
            return eval_a(n - 1) + 1;
        idx_t k      = n - p;
        idx_t mirror = p - k;
        return (seq_t)(m + 1) * eval_a(mirror);
    }

    if (known[n])
        return cache[n];

    idx_t p = 1;
    idx_t m = 0;
    /* find p = 2^m = highest power of two ≤ n */
    while (p <= n >> 1) {
        p <<= 1;
        ++m;
    }

    seq_t result;
    if (p == n) {                     /* n is a power of two */
        result = eval_a(n - 1) + 1;
    } else {                           /* general case          */
        idx_t k      = n - p;
        idx_t mirror = p - k;
        result = (seq_t)(m + 1) * eval_a(mirror);
    }

    cache[n]   = result;
    known[n]   = 1;
    return result;
}

/* public entry point required by the framework */
seq_t a(idx_t n)
{
    return eval_a(n);
}
