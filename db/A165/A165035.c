/*
 * A165035  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-5 Kaprekar map in A165032.
 * https://oeis.org/A165035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3125, u16 n <= 3125, u32 n <= 3125, u64 n <= 3125,
 *            u128 n <= 3125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = length of preperiodic part + cycle length of the
   trajectory of n under the base‑5 Kaprekar map (A165032). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static buffer reused between calls */
    static uint64_t *seen = NULL;
    static size_t seen_cap = 0;

    size_t len = 0;
    uint64_t cur = (uint64_t)n;   /* starting value */

    while (1) {
        /* look for cur among previously seen values */
        size_t i;
        for (i = 0; i < len; ++i)
            if (seen[i] == cur)
                break;

        if (i < len)                /* repetition detected */
            return (seq_t)len;

        /* grow buffer if necessary */
        if (len == seen_cap) {
            size_t newcap = seen_cap ? seen_cap << 1 : 16;
            uint64_t *tmp = (uint64_t *)realloc(seen, newcap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;   /* allocation failure, fallback */
            seen = tmp;
            seen_cap = newcap;
        }

        /* record current value */
        seen[len++] = cur;

        /* compute next value via base‑5 Kaprekar map */
        uint64_t temp = cur;
        unsigned int cnt[5] = {0};

        if (temp == 0) {
            cnt[0] = 1;
        } else {
            while (temp > 0) {
                unsigned d = (unsigned)(temp % 5);
                ++cnt[d];
                temp /= 5;
            }
        }

        uint64_t asc = 0, desc = 0;
        /* ascending digits */
        for (unsigned d = 0; d <= 4; ++d)
            for (unsigned c = 0; c < cnt[d]; ++c)
                asc = asc * 5 + d;
        /* descending digits */
        for (int d = 4; d >= 0; --d)
            for (unsigned c = 0; c < cnt[d]; ++c)
                desc = desc * 5 + d;

        cur = desc - asc;   /* non‑negative */
    }
}
