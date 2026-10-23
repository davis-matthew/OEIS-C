/*
 * A044183  Numbers k such that the string 6,3 occurs in the base 7
 *          representation of k but not of k-1.
 * https://oeis.org/A044183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A044183 – Numbers k such that the string 6,3 occurs in the base
 *            7 representation of k but not of k-1. Offset: 1
 * Range:     u8 n <= 5, u16 n <= 1507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;      /* cached terms of the sequence    */
static size_t  seq_cap   = 0;        /* current allocated capacity     */
static size_t  seq_len   = 0;        /* number of terms stored          */
static idx_t   current_k = 1;        /* next candidate k to test       */
static bool    prev_has63 = false;   /* whether previous k had 63       */

seq_t a(idx_t n)
{
    /* Expand cache until it contains the first n terms. */
    while (seq_len < n) {
        bool has63 = false;
        idx_t tmp   = current_k;
        int   last  = -1;           /* digit seen in the previous (less
                                       significant) step  */
        /* Scan digits from least significant to most.  In this order
           a base‑7 pattern “6,3” will be seen as “3” followed by “6”. */
        while (tmp > 0) {
            int d = (int)(tmp % 7);
            tmp /= 7;
            if (last == 3 && d == 6)
                has63 = true;
            last = d;
        }

        /* Store k if it has the pattern and the previous number did not. */
        if (has63 && !prev_has63) {
            if (seq_len + 1 > seq_cap) {
                size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(seq_cache,
                                        new_cap * sizeof(seq_t));
                seq_cache = new_ptr;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = current_k;
        }
        prev_has63 = has63;
        current_k++;
    }
    return seq_cache[(size_t)(n - 1)];
}
