/*
 * A131989  Start with the symbol **|* and for each iteration replace * with
 *          **|*. This sequence is the number of *'s between each dash.
 * https://oeis.org/A131989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of '*' symbols between the n‑th and (n+1)‑th '|'
 *            in the infinite word obtained from the substitution 1 → 1121 , 2
 *            → 2 (where 1 represents '*', 2 represents '|'), starting from
 *            the seed 2 1. This function implements the iterative
 *            substitution until the (n+1)‑st bar has appeared and t...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1 = '*', 2 = '|' */
    const uint8_t STAR = 1;
    const uint8_t BAR  = 2;

    /* start with the word "2 1" (i.e., '|*') */
    size_t wlen = 2;
    uint8_t *w = (uint8_t *)malloc(wlen * sizeof(*w));
    if (!w) return 0;          /* out of memory; return 0 as sentinel */
    w[0] = BAR;
    w[1] = STAR;

    /* count the number of bars (fixed points of the morphism) */
    idx_t bar_count = 1;       /* we already have one bar */

    /* generate words until we have at least n+1 bars (so we can count
       the stars between bar n and bar n+1) */
    while (bar_count < n + 1) {
        size_t next_len = 0;
        /* Each STAR expands to 4 symbols, so an upper bound for the
           new length is wlen * 4. */
        uint8_t *next = (uint8_t *)malloc(wlen * 4 * sizeof(*next));
        if (!next) { free(w); return 0; }

        for (size_t i = 0; i < wlen; ++i) {
            if (w[i] == STAR) {
                /* 1 → 1121 */
                next[next_len++] = STAR;   /* 1 */
                next[next_len++] = STAR;   /* 1 */
                next[next_len++] = BAR;    /* 2 */
                next[next_len++] = STAR;   /* 1 */
                bar_count++;               /* one new bar created */
            } else { /* w[i] == BAR */
                /* 2 → 2 */
                next[next_len++] = BAR;
            }
        }
        free(w);
        w = next;
        wlen = next_len;
    }

    /* Now scan the word to find the nth block of stars. */
    idx_t seen_bars = 0;
    seq_t stars = 0;
    for (size_t i = 0; i < wlen; ++i) {
        if (w[i] == STAR) {
            stars++;
        } else { /* BAR */
            seen_bars++;
            if (seen_bars == n + 1) {
                free(w);
                return (seq_t)stars;
            }
            stars = 0;
        }
    }

    /* Should never be reached if the word contains enough bars. */
    free(w);
    return 0;
}
