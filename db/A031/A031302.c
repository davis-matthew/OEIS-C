/*
 * A031302  Position of n-th 3 in A031298.
 * https://oeis.org/A031302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Position of the n‑th 3 in the concatenation
   of the natural numbers written in reverse order
   (e.g. 1 2 3 4 5 … becomes 1 2 3 4 5 … ;
   10 11 12 becomes 01 11 21, etc.).
   a(1) = 3, a(2) = 16, a(3) = 36, … */

/* This routine scans the natural numbers in order, writes each
   number in reverse (least significant digit first), keeps a
   running index of the overall position in the concatenated
   string, and records the positions where the digit '3' is
   encountered.  It lazily builds a cache so that repeated
   calls to a(n) are fast and independent of the order of
   the calls. */

#include <stdlib.h>

/* static cache for positions of 3’s */
static seq_t *positions = NULL;
static idx_t  pos_len  = 0; /* number of positions stored */
static idx_t  pos_cap  = 0; /* allocated capacity           */
static uint64_t cur_num = 1;    /* next natural number to process */
static idx_t   cur_pos = 1;     /* next global position in string */

/* Grow the cache until it can hold at least n positions. */
static void ensure_cache(idx_t n)
{
    char digits[32];
    while (pos_len < n) {
        /* Convert cur_num to decimal digits (least significant first). */
        uint64_t tmp = cur_num;
        int dcnt = 0;
        if (tmp == 0) {
            digits[dcnt++] = '0';
        } else {
            while (tmp > 0) {
                digits[dcnt++] = (char)((tmp % 10) + '0');
                tmp /= 10;
            }
        }

        /* Process digits in reverse order: least significant to most. */
        for (int i = 0; i < dcnt; ++i) {
            if (digits[i] == '3') {
                /* Ensure capacity for the new position. */
                if (pos_len == pos_cap) {
                    pos_cap = pos_cap ? pos_cap << 1 : 1024;
                    seq_t *newp = (seq_t *)realloc(positions,
                                                   pos_cap * sizeof(seq_t));
                    /* In a realistic build we would check for NULL. */
                    positions = newp;
                }
                positions[pos_len++] = (seq_t)cur_pos;
            }
            ++cur_pos;
        }

        /* Advance to the next natural number. */
        ++cur_num;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for offset 1 */
    ensure_cache(n);
    return positions[n - 1];
}
