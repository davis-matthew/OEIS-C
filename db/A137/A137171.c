/*
 * A137171  Interleaved reading of A000749 and its first to third differences.
 * https://oeis.org/A137171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interleaved reading of A000749 and its first–third differences.
   a(n) = A[t]                     if r = 0
   a(n) = A[t+1] - A[t]            if r = 1
   a(n) = A[t+2] - 2*A[t+1] + A[t]  if r = 2
   a(n) = A[t+3] - 3*A[t+2] + 3*A[t+1] - A[t]  if r = 3
   where t = n / 4, r = n % 4 and A(k) is A000749, with
   A(0)=0, A(1)=0, A(2)=0, A(3)=1 and
   A(k)=4*A(k-1)-6*A(k-2)+4*A(k-3)  for k>3. */

#include <stdlib.h>
#include <stddef.h>  /* for size_t */

/* Static cache of A000749 values. */
static seq_t *A_cache = NULL;
static idx_t cached_len = 0;

/* Ensure that A_cache contains values up to index 'needed' (inclusive). */
static void ensure_A(idx_t needed)
{
    if (needed < cached_len)
        return;

    /* Reallocate the cache to hold new_len entries. */
    size_t new_len = (size_t)needed + 1;
    seq_t *tmp = (seq_t *)realloc(A_cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();  /* allocation failed */

    /* If this is the first allocation, initialise the first four terms. */
    if (cached_len == 0) {
        if (new_len > 0)
            tmp[0] = (seq_t)0;
        if (new_len > 1)
            tmp[1] = (seq_t)0;
        if (new_len > 2)
            tmp[2] = (seq_t)0;
        if (new_len > 3)
            tmp[3] = (seq_t)1;
    }

    /* Compute any new terms using the recurrence. */
    for (idx_t i = cached_len; i <= needed; ++i) {
        if (i >= 4) {
            tmp[i] = (seq_t)4 * tmp[i - 1]
                   - (seq_t)6 * tmp[i - 2]
                   + (seq_t)4 * tmp[i - 3];
        }
    }

    A_cache   = tmp;
    cached_len = new_len;
}

/* External reference required by the test harness. */
seq_t a(idx_t n)
{
    idx_t t = n / 4;                 /* row index */
    unsigned r = (unsigned)(n % 4);  /* column within the interleaving */

    /* We need A[t+3] to cover the most demanding column (r==3). */
    idx_t need = t + 3;
    ensure_A(need);

    switch (r) {
        case 0:
            return A_cache[t];
        case 1:
            return A_cache[t + 1] - A_cache[t];
        case 2:
            return A_cache[t + 2]
                 - (seq_t)2 * A_cache[t + 1]
                 + A_cache[t];
        case 3:
            return A_cache[t + 3]
                 - (seq_t)3 * A_cache[t + 2]
                 + (seq_t)3 * A_cache[t + 1]
                 - A_cache[t];
        default:
            return (seq_t)0;   /* unreachable */
    }
}
