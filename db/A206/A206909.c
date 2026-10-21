/*
 * A206909  Position of 2n+cos(n) when the sets {2k+cos(k)} and {2k+sin(k)}
 *          are jointly ranked.
 * https://oeis.org/A206909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of 2n+cos(n) in the joint ranking of the
 *            sequences s_k = 2k+cos(k) and t_k = 2k+sin(k). Since s_k and t_k
 *            are strictly increasing, the rank is obtained by merging the two
 *            sequences once. The algorithm keeps a running merge and records
 *            the rank of each s_k when it is encountered. I...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>   /* for size_t */

static idx_t  cur_limit   = 0;   /* last processed s index      */
static idx_t  t_next     = 1;   /* next t index to consider    */
static idx_t  cur_pos    = 1;   /* current position in merged list          */
static seq_t *rank_arr   = NULL;/* rank_arr[i] = a(i)              */
static size_t rank_cap   = 0;   /* allocated capacity              */

static void ensure_rank_cap(idx_t idx)
{
    if ((size_t)idx + 1 > rank_cap) {
        size_t new_cap = rank_cap ? rank_cap << 1 : 8;
        while ((size_t)idx + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        if (rank_cap) memcpy(tmp, rank_arr, rank_cap * sizeof(seq_t));
        free(rank_arr);
        rank_arr = tmp;
        rank_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe return */
    /* extend precomputation up to n */
    while (cur_limit < n) {
        idx_t i = cur_limit + 1;          /* next s index   */
        double s_val = 2.0 * (double)i + cos((double)i);

        /* consume all t's that are still <= s_val */
        while (1) {
            double t_val = 2.0 * (double)t_next + sin((double)t_next);
            if (t_val <= s_val) {
                cur_pos++;
                t_next++;
            } else break;
        }

        /* now s_val is the smallest, so record its rank */
        ensure_rank_cap(i);
        rank_arr[i] = (seq_t)cur_pos;
        cur_pos++;
        cur_limit = i;
    }
    return rank_arr[n];
}
