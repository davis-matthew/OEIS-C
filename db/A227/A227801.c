/*
 * A227801  Numbers n whose sum of digits, sum of square of digits and sum of
 *          cube of digits all have the same digital root.
 * https://oeis.org/A227801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose sum of digits, sum of
 *            squares of digits, and sum of cubes of digits all have the same
 *            digital root
 * Range:     u8 n <= 25, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Check whether x satisfies the condition.
 * Only the remainders modulo 9 are needed, so no overflow can happen. */
static bool valid(seq_t x)
{
    int r1 = 0, r2 = 0, r3 = 0;
    seq_t t = x;
    while (t > 0) {
        seq_t d_seq = t % 10;
        uint32_t d = (uint32_t)d_seq;          /* digit 0..9 */
        int d2 = d * d;
        int d3 = d2 * d;
        r1 += d;
        r2 += d2;
        r3 += d3;
        r1 %= 9;
        r2 %= 9;
        r3 %= 9;
        t /= 10;
    }
    /* Convert 0 remainder to 9: digital root of a positive sum that is
       divisible by 9 is 9. */
    if (r1 == 0) r1 = 9;
    if (r2 == 0) r2 = 9;
    if (r3 == 0) r3 = 9;
    return r1 == r2 && r2 == r3;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* not used; offset is 1 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* first number to test */

    /* Grow cache until it holds the n-th term */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        next_candidate += 1;
        if (valid(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort with zero. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
