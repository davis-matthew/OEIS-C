/*
 * A081083  Numbers n such that rad(n+1)=rad(n)+1, where rad(m)=A007947(m) is
 *          the squarefree kernel of m.
 * https://oeis.org/A081083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that rad(n+1)=rad(n)+1
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    static idx_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next = 1;

    /* extend the cache until we have n terms */
    while (len < (size_t)n)
    {
        seq_t rad_cur  = rad(next);
        seq_t rad_next = rad(next + 1);

        if (rad_next == rad_cur + (seq_t)1)
        {
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 4 : cap << 1;
                seq = (idx_t *)realloc(seq, new_cap * sizeof(idx_t));
                if (!seq) exit(EXIT_FAILURE);
                cap = new_cap;
            }
            seq[len++] = next;
        }
        ++next;
    }

    return (seq_t)seq[n - 1];
}
