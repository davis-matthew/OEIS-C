/*
 * A039337  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑8 representation has the same
 *            * nonzero number of 1's and 3's. The function builds the
 *            sequence incrementally * and memoises results
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *seq = NULL;          /* array of found numbers */
    static size_t seq_len = 0;        /* how many numbers stored */
    static size_t seq_cap = 0;        /* capacity of seq array */
    static seq_t cur = 1;             /* next candidate to test */

    if (n == 0)                        /* undefined for n=0, return 0 */
        return 0;

    /* initialise array on first call */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) return 0;      /* allocation failed */
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* count digits 1 and 3 in base 8 of the candidate */
        seq_t x = cur;
        int cnt1 = 0, cnt3 = 0;
        while (x != (seq_t)0) {
            unsigned d = (unsigned)(x % (seq_t)8);
            if (d == 1) ++cnt1;
            else if (d == 3) ++cnt3;
            x = x / (seq_t)8;
        }
        if (cnt1 == cnt3 && cnt1 > 0) {
            /* store the found number */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;      /* allocation failed */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = cur;
        }
        ++cur;     /* move to next integer */
    }

    return seq[n - 1];
}
