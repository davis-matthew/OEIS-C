/*
 * A104108  a(1) = 1, c(1)=1, if A(k) = sequence of first c(k) terms and if
 *          B(k) is A(k) with 0's and 1's exchanged, then A(k+1) = A(k)B(k)
 *          and c(k+1)= 2*c(k) if a(k) = 1, A(k+1) = A(k),0,B(k) and c(k+1)=
 *          2*c(k)+1 if a(k) = 0.
 * https://oeis.org/A104108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A104108: a(1)=1.
   For k ≥ 1 let A(k) be the first c(k) terms of the sequence, and let
   B(k) be A(k) with 0↔1 exchanged.  Then
      A(k+1) = A(k)B(k)               if a(k) = 1,
      A(k+1) = A(k),0,B(k)            if a(k) = 0.
   Here c(k+1)=2·c(k) when a(k)=1, and c(k+1)=2·c(k)+1 when a(k)=0.
   The infinite binary sequence produced in this way is a(n).
   The function a(idx_t n) returns the n‑th term (n≥1); it is efficient
   for n up to ≈10 000.
*/

#include <stdlib.h>   /* malloc, free, size_t */

static seq_t *seq = NULL;      /* stored prefix of the sequence             */
static idx_t  seq_len = 0;     /* current number of stored elements          */
static idx_t  step    = 1;     /* index k (1‑based) for the next construction */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside the defined offset */

    /* Initialise with a(1) = 1 on first call */
    if (seq_len == 0) {
#ifdef __cplusplus
        seq = new seq_t[1];
#else
        seq = (seq_t*)malloc(sizeof(seq_t));
#endif
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Grow the stored prefix until it reaches the requested index */
    while (seq_len < n) {
        seq_t bit = seq[step - 1];        /* a(step) – 0 or 1 */
        idx_t new_len = (bit == (seq_t)1)
                         ? seq_len + seq_len
                         : seq_len + seq_len + 1;

        seq_t *new_seq = NULL;
#ifdef __cplusplus
        new_seq = new seq_t[new_len];
#else
        new_seq = (seq_t*)malloc((size_t)new_len * sizeof(seq_t));
#endif

        /* copy the old sequence */
        for (idx_t i = 0; i < seq_len; ++i)
            new_seq[i] = seq[i];

        /* insert a zero between the two halves if a(step)=0 */
        if (bit == (seq_t)0)
            new_seq[seq_len] = (seq_t)0;

        /* append the bitwise complement of the old sequence */
        idx_t offset = (bit == (seq_t)1) ? seq_len : seq_len + 1;
        for (idx_t i = 0; i < seq_len; ++i)
            new_seq[offset + i] = seq[i] ^ (seq_t)1;

        /* free the old array and update state */
#ifdef __cplusplus
        delete[] seq;
#else
        free(seq);
#endif
        seq      = new_seq;
        seq_len  = new_len;
        ++step;
    }

    return seq[n - 1];                    /* zero‑based indexing */
}
