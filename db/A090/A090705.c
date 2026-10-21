/*
 * A090705  Lexicographically earliest permutation of squarefree numbers with
 *          mu(a(n)) = -(-1)^n, where mu = A008683.
 * https://oeis.org/A090705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A102510(A005117(n)) (lexicographically earliest
 *            permutation of squarefree numbers with μ(a(n)) = -(-1)^n)
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"

/* Cached sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Queues holding squarefree numbers with μ = +1 (even) and μ = -1 (odd) */
static seq_t *even_q = NULL;
static size_t even_head = 0, even_tail = 0, even_cap = 0;

static seq_t *odd_q  = NULL;
static size_t odd_head  = 0, odd_tail  = 0, odd_cap  = 0;

/* Highest integer that has been examined for squarefreeness */
static idx_t checked_up_to = 0;

/* Grow a dynamic array to new capacity.  The caller must ensure that
   there is enough space to store new elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t new_cap)
{
    *cap = new_cap;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
}

/* Fill the requested parity queue until it contains at least
   one element.  Numbers are tested in increasing order. */
static void ensure_parity_queue(bool want_even)
{
    while ( (want_even ? (even_head == even_tail) : (odd_head == odd_tail)) )
    {
        ++checked_up_to;
        seq_t mu = mobius(checked_up_to);
        if (mu == (seq_t)1)          /* even parity (μ = +1) */
        {
            if (even_tail == even_cap)
                grow_array(&even_q, &even_cap, even_cap ? even_cap * 2 : 4);
            even_q[even_tail++] = (seq_t)checked_up_to;
        }
        else if (mu == (seq_t)(-1))  /* odd parity (μ = -1) */
        {
            if (odd_tail == odd_cap)
                grow_array(&odd_q, &odd_cap, odd_cap ? odd_cap * 2 : 4);
            odd_q[odd_tail++] = (seq_t)checked_up_to;
        }
        /* μ = 0 (non–squarefree) is ignored */
    }
}

/* Expand the main cache to contain at least the first n terms. */
static void ensure_cache(idx_t n)
{
    while (seq_len < n)
    {
        idx_t pos      = seq_len + 1;          /* 1‑based index of the next term */
        bool want_even = (pos % 2 == 1);        /* μ = +1 on odd positions */
        ensure_parity_queue(want_even);

        seq_t val = want_even ? even_q[even_head++] : odd_q[odd_head++];
        if (seq_len == seq_cap)
            grow_array(&seq_cache, &seq_cap, seq_cap ? seq_cap * 2 : 4);
        seq_cache[seq_len++] = val;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */
    ensure_cache(n);
    return seq_cache[n - 1];
}
