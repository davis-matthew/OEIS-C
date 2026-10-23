/*
 * A043920  Numbers k such that 1 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_adjacent_15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_15.h"

/* Returns the nth integer k (n ≥ 1) such that the base‑7 representation
   of k contains the adjacent digits 1 and 5 (either "15" or "51") and
   the base‑7 representation of k+1 does *not* contain those adjacent digits. */

static bool candidate(seq_t k)
{
    return has_adjacent_15(k) && !has_adjacent_15(k + 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; caller should not request n==0 */

    /* Static state for memoisation across arbitrary call order. */
    static seq_t *seq = NULL;    /* array of found candidates   */
    static size_t cap = 0;       /* capacity of the array       */
    static size_t used = 0;      /* number of candidates stored */
    static seq_t next_test = 1;  /* next integer to test        */

    /* Initialise storage lazily. */
    if (cap == 0)
    {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate additional candidates until we have at least n of them. */
    while (used < n)
    {
        seq_t k = next_test;
        next_test = k + 1;      /* advance for the next test */

        if (candidate(k))
        {
            if (used >= cap)
            {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp)   /* allocation failed – abort */
                {
                    free(seq);
                    seq = NULL;
                    cap = 0;
                    used = 0;
                    return 0;
                }
                seq = tmp;
                cap = newcap;
            }
            seq[used++] = k;
        }
    }

    return seq[n - 1];
}
