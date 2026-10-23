/*
 * A189572  Fixed point of the morphism 0->01, 1->001.
 * https://oeis.org/A189572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(alpha*n+beta) - floor(alpha*(n-1)+beta) * with
 *            alpha = sqrt(2)-1, beta = 1 - sqrt(2)/2 * Sequence defined by
 *            the morphism 0->01, 1->001 (fixed point)
 * Range:     u8 n <= 116, u16 n <= 116, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *seq_buf = NULL; /* buffer holding symbols (0 or 1) */
static size_t seq_len = 0;      /* number of symbols stored */

/* Returns the n-th term (1‑indexed).  Undefined for n==0; return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* 0 is not part of the sequence */
    size_t need = (size_t)n;

    /* Initialize the sequence with the seed “0” on first call or when buffer cleared. */
    if (seq_len == 0) {
        seq_buf = (uint8_t *)malloc(need);
        if (!seq_buf) return (seq_t)0;
        seq_buf[0] = 0;
        seq_len = 1;
    }

    /* Expand until we have enough symbols. */
    while (seq_len < need) {
        uint8_t *new_buf = (uint8_t *)malloc(need);
        if (!new_buf) {
            free(seq_buf);
            seq_buf = NULL;
            seq_len = 0;
            return (seq_t)0;
        }

        size_t i = 0;          /* index in current buffer */
        size_t j = 0;          /* index in new buffer */
        while (i < seq_len && j < need) {
            uint8_t v = seq_buf[i++];
            if (v == 0) {              /* 0 → 01 */
                new_buf[j++] = 0;
                if (j < need) new_buf[j++] = 1;
            } else {                  /* 1 → 001 */
                new_buf[j++] = 0;
                if (j < need) new_buf[j++] = 0;
                if (j < need) new_buf[j++] = 1;
            }
        }

        free(seq_buf);
        seq_buf = new_buf;
        seq_len = j;
    }

    return (seq_t)seq_buf[need - 1];
}
