/*
 * A123069  Odd positive integers that are not the sum of four positive
 *          squares.
 * https://oeis.org/A123069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A123069
 *  Odd positive integers that are not the sum of four positive squares.
 *
 *  This implementation generates the sequence lazily, storing the
 *  values in a static array.  For each candidate odd integer it
 *  checks whether it can be written as a^2+b^2+c^2+d^2 with
 *  a,b,c,d ≥ 1.  The check is performed by iterating over a and
 *  b, computing the remaining sum r2 = n‑a²‑b² and then testing
 *  whether r2 can be expressed as c²+d² with c,d ≥ 1.  The latter
 *  test uses an integer square‑root routine written in terms of
 *  the generic seq_t type.
 *
 *  The routine grows the cache as needed and supports arbitrary
 *  calls with n in any order.  All arithmetic is performed using
 *  seq_t and idx_t, and the implementation is valid C11/C++17.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

/* Integer square root for seq_t: floor(sqrt(x)) */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Check whether n can be written as a sum of four positive squares. */
static bool is_four_pos_squares(seq_t n)
{
    if (n < 4) return false;          /* minimum sum is 1+1+1+1 = 4 */

    for (idx_t a = 1; (seq_t)a * (seq_t)a < n; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        seq_t r1 = n - a2;
        if (r1 < 3) continue;           /* need at least 1+1+1 */
        for (idx_t b = 1; (seq_t)b * (seq_t)b < r1; ++b) {
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t r2 = r1 - b2;
            if (r2 < 2) continue;       /* need at least 1+1 */
            seq_t sqrt_r2 = isqrt_seq(r2);
            for (idx_t c = 1; c <= sqrt_r2; ++c) {
                seq_t c2 = (seq_t)c * (seq_t)c;
                seq_t r3 = r2 - c2;
                if (r3 <= 0) continue;
                seq_t d = isqrt_seq(r3);
                if (d >= 1 && d * d == r3) return true;
            }
        }
    }
    return false;
}

/* Cache for sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_odd = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, offset starts at 1 */

    while (cache_len < n) {
        seq_t cand = (seq_t)next_odd;
        if (!is_four_pos_squares(cand)) {
            /* Grow cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        next_odd += 2;   /* only odd candidates */
    }
    return cache[n - 1];
}
