/*
 * A100619  Fixed point of the morphism 1 -> 12, 2 -> 31, 3 -> 1, starting
 *          from a(1) = 1.
 * https://oeis.org/A100619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100619
    Fixed point of the morphism
        1 → 12
        2 → 31
        3 → 1
    starting from a(1) = 1.
    The nth term is obtained by iteratively applying the morphism
    until a word of length ≥ n is produced, then returning the nth
    symbol of that word.                                                         */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;            /* undefined for n <= 0, but harmless */

    /* start with the seed word "1" */
    unsigned char *cur = (unsigned char*)malloc(sizeof(unsigned char));
    cur[0] = 1;
    idx_t cur_len = 1;

    /* iterate until we have at least n symbols */
    while (cur_len < n)
    {
        idx_t next_len = 0;
        /* each symbol expands to at most 2 symbols, so allocate enough space */
        unsigned char *next = (unsigned char*)malloc(sizeof(unsigned char) * cur_len * 2);
        for (idx_t i = 0; i < cur_len; ++i)
        {
            unsigned char v = cur[i];
            if (v == 1) {
                next[next_len++] = 1;
                next[next_len++] = 2;
            }
            else if (v == 2) {
                next[next_len++] = 3;
                next[next_len++] = 1;
            }
            else { /* v == 3 */
                next[next_len++] = 1;
            }
        }
        free(cur);
        cur = next;
        cur_len = next_len;
    }

    unsigned char res = cur[n - 1];
    free(cur);
    return (seq_t)res;
}
