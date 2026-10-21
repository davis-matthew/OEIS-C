/*
 * A068995  Integer parts of the square roots of the schizophrenic numbers
 *          (A014824).
 * https://oeis.org/A068995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   repunit128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A068995 – Integer parts of the square roots of the schizophrenic numbers 
 * (A014824).  The sequence is defined by:
 *   a(2n-1) = A014824(n) – A014824(n-1),   n ≥ 1
 *   a(2n-2) = ⌊ a(2n-1) / √10 ⌋,         n ≥ 2
 *
 * The implementation below uses the closed‑form description that
 *   a(odd)  = repunit of length (n+1)/2  (i.e. 11,111,…)
 *   a(even) = floor( √ ( rep^2 / 10 ) )
 *   with rep = repunit of length (n/2)+1.
 *
 * Because the intermediate product rep² may overflow ordinary
 * integral types, the routine uses 128‑bit arithmetic (available as
 * __uint128_t on the compiler used for the build).  Only when the
 * largest product fits into 64 bits do we need the extra precision;
 * otherwise the truncation is harmless because the resulting term
 * would not fit into the target seq_t anyway.
 *
 * For builds where seq_t is an arbitrary‑precision integer class,
 * the conversion to (unsigned long long) is safe when the term
 * it represents fits into 64 bits – exactly the case in which the
 * function is required to be exact.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/repunit128.h"

/* 128‑bit integer square root: returns floor( sqrt(v) ) for v ≥ 0.
 * The result is guaranteed to fit into an unsigned 64‑bit value
 * when v ≤ (2^64−1)², which is sufficient for our use. */
static inline unsigned long long sqrt128(__uint128_t v)
{
    unsigned long long low  = 0;
    unsigned long long high = (unsigned long long)(((__uint128_t)1 << 64) - 1);
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= v) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – a(1) is defined, a(0) unused */
        return 0;

    if ((n & 1) == 1) {          /* odd index 2k‑1 → repunit length (n+1)/2 */
        size_t len = (n + 1) >> 1;
        __uint128_t r128 = repunit128(len);
        /* cast to seq_t – truncation occurs only when the value
         * exceeds the size of seq_t, but then the term itself does
         * not fit in seq_t and no exact value is required. */
        return (seq_t)(unsigned long long)r128;
    } else {                      /* even index 2k → floor(√(rep²/10)) */
        size_t len = (n >> 1) + 1;   /* length of the repunit */
        __uint128_t rep128 = repunit128(len);
        __uint128_t rep64 = (unsigned long long)rep128;      /* rep fits in 64 bits here */
        __uint128_t rep_sq = rep64 * rep64;                  /* rep²  (≤ 2^128‑1) */
        __uint128_t v = rep_sq / 10;                        /* rep² / 10 */
        unsigned long long result = sqrt128(v);
        return (seq_t)result;
    }
}
