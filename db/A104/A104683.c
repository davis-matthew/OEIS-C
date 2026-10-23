/*
 * A104683  Interlaces "2*n^2 - 1 is a square" with NSW numbers.
 * https://oeis.org/A104683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = y_k, a(2k+1) = x_k for k ≥ 0, where (x_0,y_0) = (1,1)
 *            and (x_k,y_k) = (3x_{k-1}+4y_{k-1}, 2x_{k-1}+3y_{k-1})
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Arrays holding the k‑th pair (x_k , y_k).  */
static seq_t *x_arr = NULL;
static seq_t *y_arr = NULL;
static idx_t   pair_cnt = 0;      /* number of pairs currently stored */

/* Ensure that pairs 0 … target exist.  */
static void
ensure_pairs(idx_t target)
{
    /* Allocate or grow storage.  */
    size_t new_cap = pair_cnt;
    if (new_cap == 0)
        new_cap = 1;
    while (new_cap <= (size_t)target)
        new_cap <<= 1;

    if (new_cap > pair_cnt) {
        seq_t *nx = (seq_t *)realloc(x_arr, new_cap * sizeof(seq_t));
        seq_t *ny = (seq_t *)realloc(y_arr, new_cap * sizeof(seq_t));
        x_arr = nx;
        y_arr = ny;
        /* pair_cnt remains the count of already generated pairs. */
    }

    /* Generate missing pairs.  */
    if (pair_cnt == 0) {
        /* Base case (x_0,y_0).  */
        x_arr[0] = (seq_t)1;
        y_arr[0] = (seq_t)1;
        pair_cnt = 1;
    }

    while (pair_cnt <= target) {
        seq_t x = x_arr[pair_cnt - 1];
        seq_t y = y_arr[pair_cnt - 1];
        /* Next pair: (x_k , y_k) = (3x + 4y , 2x + 3y). */
        x_arr[pair_cnt] = x * (seq_t)3 + y * (seq_t)4;
        y_arr[pair_cnt] = x * (seq_t)2 + y * (seq_t)3;
        ++pair_cnt;
    }
}

seq_t
a(idx_t n)
{
    /* Even n  -> y_{n/2},  odd n -> x_{n/2}.  */
    idx_t k = n >> 1;          /* floor(n/2) */
    ensure_pairs(k);
    if ((n & (idx_t)1) == 0)
        return y_arr[k];
    else
        return x_arr[k];
}
