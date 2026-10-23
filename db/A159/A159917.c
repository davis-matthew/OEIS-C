/*
 * A159917  Fixed point of the morphism 0 -> 01, 1 -> 2, 2 -> 01, starting
 *          from a(0) = 0.
 * https://oeis.org/A159917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th symbol (0,1,2) of the fixed point of the
 *            morphism 0 → 01, 1 → 2, 2 → 01 starting with the single symbol 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use 0/1/2 stored as unsigned char because only three values are needed. */
    size_t target = (size_t)n;          /* target position */
    size_t len = 1;                     /* current length of the word */
    uint8_t *cur = (uint8_t *)malloc(len);
    if (!cur) return (seq_t)0;          /* OOM guard */
    cur[0] = 0;                         /* start from symbol 0 */

    /* Expand the word until it is long enough to contain position n. */
    while (len <= target) {
        /* Compute length of next word. */
        size_t next_len = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t v = cur[i];
            if (v == 1)
                ++next_len;          /* 1 → 2 */
            else
                next_len += 2;       /* 0 or 2 → 0 1 */
        }

        /* Allocate buffer for next word. */
        uint8_t *next = (uint8_t *)malloc(next_len);
        if (!next) {
            free(cur);
            return (seq_t)0;          /* OOM guard */
        }

        /* Fill the next word. */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t v = cur[i];
            if (v == 1) {
                next[pos++] = 2;      /* 1 → 2 */
            } else {
                next[pos++] = 0;      /* 0 or 2 → 0 1 */
                next[pos++] = 1;
            }
        }

        /* Move to the next iteration. */
        free(cur);
        cur = next;
        len = next_len;
    }

    /* Retrieve the requested symbol. */
    seq_t result = (seq_t)cur[(size_t)n];
    free(cur);
    return result;
}
