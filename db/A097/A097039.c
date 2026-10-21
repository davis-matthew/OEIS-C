/*
 * A097039  a(n) = Sum_{i=0..n} i*L(i), where L = A000032.
 * https://oeis.org/A097039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i*L(i) where L is the Lucas sequence
 *            (L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)). Equivalently a(n) =
 *            n*L(n+2) - L(n+3) + 4
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static caches for Lucas numbers and a(n) values */
static seq_t *Lucas = NULL;     /* Lucas numbers, indices 0.. */
static idx_t  Lucas_len = 0;    /* size of allocated Lucas array */

static seq_t *A = NULL;        /* a(n) values, indices 0.. */
static idx_t  A_len = 0;        /* size of allocated A array */

/* Ensure Lucas[i] exists for all i < need (need >= 1) */
static void ensureLucas(idx_t need)
{
    if (Lucas_len >= need)
        return;

    idx_t new_len = need;
    seq_t *tmp = (seq_t *)realloc(Lucas, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    Lucas = tmp;

    /* initialise new entries */
    if (Lucas_len == 0) {
        Lucas[0] = (seq_t)2;
        Lucas_len = 1;
        if (new_len > 1) {
            Lucas[1] = (seq_t)1;
            Lucas_len = 2;
        }
    }

    for (idx_t i = Lucas_len; i < new_len; ++i) {
        Lucas[i] = Lucas[i-1] + Lucas[i-2];
    }
    Lucas_len = new_len;
}

/* Ensure a[i] exists for all i < need (need >= 1, i.e. we need up to i = need-1) */
static void ensureA(idx_t need)
{
    if (A_len >= need)
        return;

    idx_t new_len = need;
    seq_t *tmp = (seq_t *)realloc(A, new_len * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;

    /* Make sure we have Lucas still enough for the new indices.
       We need Lucas[i+3] for i = new_len-1 → require index new_len+2.   */
    ensureLucas(new_len + 3);

    for (idx_t i = A_len; i < new_len; ++i) {
        seq_t n_seq = (seq_t)i;
        seq_t val = n_seq * Lucas[i+2] - Lucas[i+3] + (seq_t)4;
        A[i] = val;
    }
    A_len = new_len;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* We need to have a[0..n] computed */
    ensureA(n + 1);
    return A[n];
}
