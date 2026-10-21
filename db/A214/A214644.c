/*
 * A214644  Indices of c in the sequence closed under a -> abc, b -> ab, c ->
 *          b . Start with a.
 * https://oeis.org/A214644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A214644:  indices of the symbol 'c'
 *  in the infinite word obtained by iterating the morphism
 *      a → abc
 *      b → ab
 *      c → b
 *  starting from the single letter 'a'.
 *  The function returns the n‑th (1‑based) such index.
 *  The algorithm keeps track of positions that have already
 *  produced a 'c' to avoid double counting, expanding the word
 *  step by step until the n‑th 'c' is found.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, safeguard */

    /* current word (string) and a flag array for seen 'c' positions */
    char *cur   = (char *)malloc(1);
    bool *seen  = (bool *)malloc(1);
    cur[0]      = 'a';
    seen[0]     = false;
    idx_t cur_len = 1;

    /* count of c's already registered */
    idx_t c_found = 0;

    while (1)
    {
        /* compute length of next word */
        idx_t next_len = 0;
        for (idx_t i = 0; i < cur_len; ++i)
        {
            switch (cur[i])
            {
                case 'a': next_len += 3; break;
                case 'b': next_len += 2; break;
                default : next_len += 1; /* 'c' -> 'b' */
            }
        }

        /* allocate next word and flag array */
        char *next          = (char *)malloc((size_t)next_len);
        bool *next_seen     = (bool *)malloc((size_t)next_len);

        /* copy the already seen flags from the previous word */
        memcpy(next_seen, seen, (size_t)cur_len * sizeof(bool));
        /* remaining positions are initially false */
        if (next_len > cur_len)
            memset(next_seen + cur_len, 0, (size_t)(next_len - cur_len));

        /* expand current word into next word, counting new 'c's */
        bool done   = false;
        idx_t result = 0;
        idx_t write = 0;

        #define APPEND(ch) do { \
                next[write] = (ch); \
                if ((ch) == 'c' && !next_seen[write]) { \
                    next_seen[write] = true; \
                    ++c_found; \
                    if (c_found == n) { result = write + 1; done = true; } \
                } \
                ++write; \
            } while (0)

        for (idx_t i = 0; i < cur_len && !done; ++i)
        {
            switch (cur[i])
            {
                case 'a':
                    APPEND('a');
                    if (done) break;
                    APPEND('b');
                    if (done) break;
                    APPEND('c');
                    break;
                case 'b':
                    APPEND('a');
                    if (done) break;
                    APPEND('b');
                    break;
                default: /* 'c' -> 'b' */
                    APPEND('b');
                    break;
            }
        }

        /* free resources of the old word */
        free(cur);
        free(seen);

        if (done)   /* nth 'c' found */
        {
            free(next);
            free(next_seen);
            return (seq_t)result;
        }

        /* prepare for next iteration */
        cur      = next;
        seen     = next_seen;
        cur_len  = next_len;
    }
}
