/*
 * A106751  Define the morphism f: 1->111112, 2->22223, 3->1; sequence gives
 *          trajectory of 1 under f.
 * https://oeis.org/A106751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol (0‑based) of the infinite word produced *
 *            by iterating the morphism * f(1) = 111112 * f(2) = 22223 * f(3)
 *            = 1 * starting from the single symbol 1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t cur_len = 1, cur_cap = 1;
    seq_t *cur = (seq_t*)malloc(cur_cap * sizeof(seq_t));
    if (!cur) return (seq_t)0;      /* OOM – return a neutral value */
    cur[0] = (seq_t)1;              /* start with the symbol 1 */

    /* Expand the word until we have at least n+1 symbols. */
    while (cur_len <= (size_t)n) {
        size_t next_cap = cur_len * 6 + 1;
        seq_t *next = (seq_t*)malloc(next_cap * sizeof(seq_t));
        if (!next) { free(cur); return (seq_t)0; }

        size_t next_len = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            const seq_t c = cur[i];
            if (c == (seq_t)1) {
                next[next_len++] = (seq_t)1;
                next[next_len++] = (seq_t)1;
                next[next_len++] = (seq_t)1;
                next[next_len++] = (seq_t)1;
                next[next_len++] = (seq_t)1;
                next[next_len++] = (seq_t)2;
            } else if (c == (seq_t)2) {
                next[next_len++] = (seq_t)2;
                next[next_len++] = (seq_t)2;
                next[next_len++] = (seq_t)2;
                next[next_len++] = (seq_t)2;
                next[next_len++] = (seq_t)3;
            } else {                /* c == 3 */
                next[next_len++] = (seq_t)1;
            }
        }

        free(cur);
        cur = next;
        cur_len = next_len;
    }

    seq_t result = cur[(size_t)n];
    free(cur);
    return result;
}
