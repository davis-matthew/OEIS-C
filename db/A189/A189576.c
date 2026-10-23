/*
 * A189576  Fixed point of the morphism 0->01, 1->110.
 * https://oeis.org/A189576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th symbol (0 or 1) in the fixed point of the *
 *            morphism 0->01, 1->110, with indexing starting at 1
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U)          /* undefined for n = 0; return 0 for safety */
        return 0;

    /* target index (1‑based) – safe to cast to size_t for n ≤ 64 bit */
    const size_t target = (size_t)n;

    /* start with the word "0" */
    uint8_t *seq = (uint8_t *)malloc(sizeof(uint8_t));
    if (seq == NULL)
        return 0;        /* allocation failure → 0 */

    seq[0] = 0U;
    size_t len = 1U;

    /* grow the word until it reaches or exceeds the target index */
    while (len < target) {
        /* compute new length: each 0 expands to 2 symbols, each 1 to 3 */
        size_t new_len = 0U;
        for (size_t i = 0; i < len; ++i)
            new_len += (seq[i] == 0U) ? 2U : 3U;

        uint8_t *next = (uint8_t *)malloc(new_len * sizeof(uint8_t));
        if (next == NULL) {      /* allocation failure */
            free(seq);
            return 0;
        }

        /* build the expansion according to the morphism */
        size_t pos = 0U;
        for (size_t i = 0; i < len; ++i) {
            if (seq[i] == 0U) {
                next[pos++] = 0U;
                next[pos++] = 1U;
            } else /* seq[i] == 1U */ {
                next[pos++] = 1U;
                next[pos++] = 1U;
                next[pos++] = 0U;
            }
        }

        free(seq);
        seq = next;
        len = new_len;
    }

    /* return the desired symbol, converted to seq_t */
    seq_t result = (seq_t)seq[target - 1U];
    free(seq);
    return result;
}
