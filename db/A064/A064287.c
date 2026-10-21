/*
 * A064287  Number of cyclotomic cosets C of 2 mod 2n+1 such that -C is not
 *          equal to C, divided by 2.
 * https://oeis.org/A064287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 4094, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For m = 2*n + 1, enumerate all cyclotomic cosets of 2
 * modulo m.  Count the cosets C for which -C != C, then return that count
 * divided by 2.  This yields A064287. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (odd positive integer) */
    idx_t m = (idx_t)2 * n + (idx_t)1;

    /* visited marks residues already assigned to a coset */
    bool *visited = (bool *)calloc((size_t)m, sizeof(bool));
    if (!visited) return (seq_t)0;          /* out of memory guard */

    seq_t count = (seq_t)0;                 /* number of asymmetric cosets */

    for (idx_t i = (idx_t)1; i < m; ++i)
    {
        if (!visited[i])
        {
            /* build the coset C = { i, 2*i, 4*i, … } mod m */
            idx_t *coset = NULL;
            size_t sz = 0, cap = 0;
            idx_t x = i;

            do
            {
                if (sz == cap)
                {
                    cap = (cap == 0) ? (size_t)1 : cap << 1;
                    idx_t *tmp = (idx_t *)realloc(coset, cap * sizeof(idx_t));
                    if (!tmp)
                    {
                        free(coset);
                        free(visited);
                        return (seq_t)0;   /* out of memory guard */
                    }
                    coset = tmp;
                }
                coset[sz++] = x;
                visited[x] = true;
                x = (x << 1) % m;          /* multiply by 2 modulo m */
            } while (x != i);

            /* check whether -C == C */
            bool same = true;
            for (size_t p = 0; p < sz && same; ++p)
            {
                idx_t y = m - coset[p];   /* -coset[p] mod m (m is odd) */
                bool found = false;
                for (size_t q = 0; q < sz; ++q)
                {
                    if (coset[q] == y)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) same = false;
            }

            if (!same) count += (seq_t)1;

            free(coset);
        }
    }

    free(visited);
    return count / ((seq_t)2);              /* divide by 2 */
}
