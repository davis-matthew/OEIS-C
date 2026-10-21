/*
 * A205710  (prime(k)-prime(j))/8, where the pairs (k,j) are given by A205705
 *          and A205706.
 * https://oeis.org/A205710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(k)-prime(j))/8, where the pairs (k,j) are given by
 *            OEIS sequences A205705 (k-values) and A205706 (j-values), i.e.
 *            for each k in increasing order, all j < k with
 *            (prime(k)-prime(j)) divisible by 8 are taken in increasing order
 *            of j
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *seq = NULL;        /* cached sequence values */
static size_t seq_cap = 0;       /* current capacity of seq array   */
static size_t seq_len = 0;       /* number of computed terms        */

/* current position in the enumeration */
static idx_t cur_k = 2;          /* next prime index k (>=2)       */
static idx_t cur_j = 1;          /* next prime index j (for cur_k) */

/* Return the next term of the sequence, advancing cur_k/cur_j.
   The function assumes cur_k and cur_j are initialised. */
static seq_t next_term(void)
{
    while (true) {
        /* Scan j from cur_j up to cur_k-1. */
        for (; cur_j < cur_k; ++cur_j) {
            seq_t diff = (seq_t)get_prime(cur_k) - (seq_t)get_prime(cur_j);
            if ((diff & 7u) == 0) {              /* diff % 8 == 0 */
                ++cur_j;                        /* next j for this k */
                return diff >> 3;                /* (prime(k)-prime(j))/8 */
            }
        }
        /* All j for current k examined; move to next k. */
        ++cur_k;
        cur_j = 1;
    }
}

/* Ensure that seq contains at least n terms. */
static void ensure_seq_len(idx_t n)
{
    if (n <= 0) return;
    while (seq_len < (size_t)n) {
        seq_t val = next_term();                  /* produce next value */
        if (seq_len >= seq_cap) {                 /* grow storage */
            seq_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) exit(EXIT_FAILURE);
        }
        seq[seq_len++] = val;                     /* store value */
    }
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined below offset */
    ensure_seq_len(n);
    return seq[n - 1];
}
