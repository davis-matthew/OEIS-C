/*
 * A104145  a(1) = 1; let A(k) = sequence of first 2^(k-1) terms; then A(k+1)
 *          is concatenation of A(k) and (A(k)-1) if a(k) is odd, or
 *          concatenation of A(k) and (A(k)+1) if a(k) is even.
 * https://oeis.org/A104145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 115, i16 n <= 115, i32 n <= 115, i64 n <= 115,
 *            i128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104145
   a(1) = 1.
   For k ≥ 1, let A(k) be the first 2^(k‑1) terms of the sequence.
   To obtain A(k+1), append to A(k) all elements of A(k) shifted by
   +1 if a(k) is even and by –1 if a(k) is odd.
   The resulting 2^k terms form A(k+1).  This recursive rule generates
   the sequence a(n).  Initial term: a(1)=1. */
#include <stdlib.h>

static seq_t *seq_buf = NULL;   /* array holding computed terms   */
static idx_t seq_len = 0;       /* current number of stored terms */
static idx_t seq_cap = 0;       /* current allocation capacity   */
static idx_t cur_k   = 1;       /* current block index k          */

/* Resize the internal array if it would not hold 'need' elements.  */
static void ensure_capacity(idx_t need)
{
    if (seq_cap >= need)
        return;
    idx_t newcap = seq_cap ? seq_cap * 2 : 8;
    while (newcap < need)
        newcap *= 2;
    seq_t *newbuf = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newbuf)          /* allocation failed – keep old buffer */
        return;
    if (seq_buf) {
        for (idx_t i = 0; i < seq_len; ++i)
            newbuf[i] = seq_buf[i];
        free(seq_buf);
    }
    seq_buf = newbuf;
    seq_cap = newcap;
}

/* Return the n‑th term of A104145 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined, safe fallback */
        return 0;

    if (seq_len == 0) {   /* first call – initialise */
        ensure_capacity(1);
        seq_buf[0] = 1;   /* a(1) */
        seq_len = 1;
        cur_k   = 1;
    }

    /* Expand until we have at least n terms. */
    while (seq_len < n) {
        /* Shift sign is determined by the parity of a(k). */
        seq_t val   = seq_buf[cur_k - 1];                     /* a(k)   */
        seq_t shift = ((val & (seq_t)1) == 0) ? (seq_t)1 : (seq_t)-1;

        idx_t old_len = seq_len;
        ensure_capacity(old_len * 2);

        /* Append shifted copy of the current block. */
        for (idx_t i = 0; i < old_len; ++i)
            seq_buf[old_len + i] = seq_buf[i] + shift;

        seq_len <<= 1;   /* double the length for the next block    */
        ++cur_k;         /* move to next block index                */
    }

    return seq_buf[n - 1]; /* zero‑based indexing */
}
