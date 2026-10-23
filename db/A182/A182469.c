/*
 * A182469  Triangle read by rows in which row n lists the odd divisors of n.
 * https://oeis.org/A182469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 822, u16 n <= 9650, u32 n <= 9650, u64 n <= 9650,
 *            u128 n <= 9650, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function returns the n-th term of OEIS A182469.  
   The sequence is produced by listing all odd divisors of each positive
   integer in increasing order, then concatenating those rows in order.
   Therefore a(n) is the element that appears at linear index n when
   the rows
      1 : 1
      2 : 1
      3 : 1 3
      4 : 1
      5 : 1
      …
   are flattened.  No primitive formula exists; the computation follows
   the definition directly.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *ap = (const seq_t *)a;
    const seq_t *bp = (const seq_t *)b;
    if (*ap < *bp) return -1;
    if (*ap > *bp) return 1;
    return 0;
}

/* Return the n‑th term of the triangle of odd divisors (triangular
   indexing starts at 1).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* safety for out‑of‑range calls */

    idx_t count = 0;        /* number of terms produced so far */

    for (idx_t r = 1; ; ++r) {
        /* Strip factors of 2 to obtain the odd part of r.  */
        uint64_t odd = r;
        while ((odd & 1ULL) == 0) odd >>= 1;

        /* Collect odd divisors of odd in tmp array.  */
        seq_t tmp[256];
        size_t tmpcnt = 0;

        for (uint64_t d = 1; d * d <= odd; d += 2) {
            if (odd % d == 0) {
                tmp[tmpcnt++] = (seq_t)d;
                uint64_t other = odd / d;
                if (other != d) {
                    tmp[tmpcnt++] = (seq_t)other;
                }
            }
        }

        /* Sort the divisors for this row.  */
        qsort(tmp, tmpcnt, sizeof(seq_t), cmp_seq_t);

        /* Walk through the sorted divisors, keeping a global counter. */
        for (size_t i = 0; i < tmpcnt; ++i) {
            ++count;
            if (count == n) {
                return tmp[i];
            }
        }
    }
}
