/*
 * A123942  The (1,4)-entry in the 4 X 4 matrix M^n, where M={{3, 2, 1, 1},
 *          {2, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}} (n>=0).
 * https://oeis.org/A123942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - a(n-3) - a(n-4) initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=15
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maximum pre‑allocated index – ample for the test limits */
    enum { MAX_PRE_ALLOC = 100000 };
    static seq_t buf[MAX_PRE_ALLOC + 1];
    static bool init = false;
    static idx_t last_computed = 3;  /* last index whose value is cached */

    /* initialise the first four terms once */
    if (!init) {
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)3;
        buf[3] = (seq_t)15;
        init = true;
    }

    /* simple quick return for the small indices */
    if (n <= 3) {
        return buf[(size_t)n];
    }

    /* if the requested value is already cached, just return it */
    if (n <= last_computed) {
        return buf[(size_t)n];
    }

    /* if the requested index exceeds our pre‑allocation, compute on‑the‑fly
       using only the last four values – this avoids allocating enormous
       buffers and still keeps O(n) time per call for very large n. */
    if (n > MAX_PRE_ALLOC) {
        seq_t a0 = buf[0];
        seq_t a1 = buf[1];
        seq_t a2 = buf[2];
        seq_t a3 = buf[3];
        seq_t cur = 0;
        for (idx_t i = 4; i <= n; ++i) {
            cur = (seq_t)4 * a3 + (seq_t)4 * a2 - a1 - a0;
            a0 = a1;  a1 = a2;  a2 = a3;  a3 = cur;
        }
        return cur;
    }

    /* otherwise extend the cache up to the requested index */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        buf[(size_t)i] = (seq_t)4 * buf[(size_t)(i - 1)] +
                         (seq_t)4 * buf[(size_t)(i - 2)] -
                         buf[(size_t)(i - 3)] -
                         buf[(size_t)(i - 4)];
    }
    last_computed = n;
    return buf[(size_t)n];
}
