/*
 * A115042  Positions of 1's in A094405, "a(n) = sum of all previous terms
 *          modulo n, a(1)=1".
 * https://oeis.org/A115042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of 1's in A094405.
   Let b(k) be defined by
     b(1) = 1,
     b(k) = (∑_{i=1}^{k-1} b(i)) mod k   for k > 1.
   This function returns a(n), the index of the nth occurrence of the
   value 1 in the sequence (b(k)).  Calls may arrive in any order;
   results are memoised. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of positions of 1's in b(k) */
    static seq_t *positions = NULL;   /* array of indices k such that b(k)=1 */
    static size_t pos_cap   = 0;      /* current capacity of the array */
    static size_t pos_len   = 0;      /* number of positions stored */
    /* static state of the b(k) sequence */
    static idx_t    processed = 0;   /* largest k whose b(k) has been computed */
    static seq_t   cumulative = 0;   /* sum_{i=1}^{processed} b(i) */

    /* generate b(k) and record positions of 1 until we have n of them */
    while (pos_len < n) {
        ++processed;                 /* new index k = processed */

        seq_t val;
        if (processed == 1) {
            val = (seq_t)1;
        } else {
            val = cumulative % (seq_t)processed;
        }

        cumulative += val;           /* update cumulative sum for next step */

        if (val == (seq_t)1) {       /* a new occurrence of 1 */
            if (pos_len == pos_cap) {/* grow the positions array if needed */
                size_t new_cap = pos_cap ? pos_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(positions, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* out of memory – unlikely in tests */
                positions = tmp;
                pos_cap   = new_cap;
            }
            positions[pos_len++] = processed;
        }
    }

    return positions[n - 1];
}
