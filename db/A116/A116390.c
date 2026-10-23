/*
 * A116390  Expansion of 1/(2*sqrt(1-4*x^2)-x-1).
 * https://oeis.org/A116390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A116390 : a(n) is the sequence obtained from the
    tridiagonal matrix iteration described in OEIS:
      a(0) = 1
      a(1) = 1
      a(2) = 5
      a(3) = 9
      a(4) = 33
      a(5) = 73
      ...
    The sequence is the leftmost column of Mⁿ·V where
    M has main diagonal [1,0,0,…], super‑diagonal [2,1,1,1,…]
    and sub‑diagonal [2,1,1,1,…], and V = [1,0,0,…].
    The implementation computes a(n) in O(n²) time, which is
    acceptable for n up to about 10 000 and works for any
    unsigned seq_t and idx_t. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate two working arrays of size n+3 (enough for all indices) */
    size_t size = (size_t)(n + 3);
    seq_t *cur  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(size, sizeof(seq_t));
    if (!cur || !next) {          /* allocation failure (unlikely) */
        return (seq_t)0;
    }

    cur[0] = (seq_t)1;            /* V[0] = 1 */

    for (idx_t step = 0; step < n; ++step) {
        /* compute M · cur → next */
        next[0] = cur[0] + cur[1] * (seq_t)2;               /* new[0] = cur[0] + 2*cur[1] */
        next[1] = cur[0] * (seq_t)2 + cur[2];              /* new[1] = 2*cur[0] + cur[2] */
        for (idx_t i = 2; i <= step + 1; ++i) {
            next[i] = cur[i - 1] + cur[i + 1];             /* new[i] = cur[i-1] + cur[i+1] */
        }

        /* swap cur and next so that cur holds the new state */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;

        /* zero out any unused entries of cur beyond step+1
           to keep the forthcoming accesses correct     */
        if (step + 2 < size) {
            memset(cur + (step + 2), 0,
                   (size - (step + 2)) * sizeof(seq_t));
        }
        /* next array will be overwritten in the next iteration */
    }

    seq_t result = cur[0];        /* a(n) is the first component */
    /* free temporary storage */
    free(cur);
    free(next);
    return result;
}
