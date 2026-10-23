/*
 * A124312  Expansion of g.f. x^3*(1 - x)/(1 - x - x^2 - x^3 - x^4 - x^5).
 * https://oeis.org/A124312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5); Initial
 *            values (offset 1): a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=1;
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 70, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;   /* array of computed terms, zero‑based */
static idx_t size = 0;       /* number of terms stored in memo */

/* Compute the nth term of A124312 (n >= 1).  */
seq_t a(idx_t n)
{
    if (n == 0)   /* not required by spec, but safe */
        return (seq_t)0;

    /* If the term is already known, return it immediately. */
    if (n <= size)
        return memo[n - 1];

    /* Allocate a larger array to hold the new terms. */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */

    /* Copy already computed terms, if any. */
    if (memo) {
        memcpy(tmp, memo, size * sizeof(seq_t));
        free(memo);
    }

    /* Zero the new part (will be filled by the recurrence). */
    for (idx_t i = size; i < new_size; ++i)
        tmp[i] = (seq_t)0;

    /* Ensure the initial terms are set (overwrite with the same values). */
    if (new_size >= 1) tmp[0] = (seq_t)0;          /* a(1) */
    if (new_size >= 2) tmp[1] = (seq_t)0;          /* a(2) */
    if (new_size >= 3) tmp[2] = (seq_t)1;          /* a(3) */
    if (new_size >= 4) tmp[3] = (seq_t)0;          /* a(4) */
    if (new_size >= 5) tmp[4] = (seq_t)1;          /* a(5) */

    /* Compute any new terms using the recurrence.  */
    idx_t start = (size > 5) ? size : 5;           /* first index that needs to be recomputed */
    for (idx_t i = start; i < new_size; ++i) {
        seq_t sum = (seq_t)0;
        sum += tmp[i - 1];
        sum += tmp[i - 2];
        sum += tmp[i - 3];
        sum += tmp[i - 4];
        sum += tmp[i - 5];
        tmp[i] = sum;
    }

    /* Update the global state. */
    memo = tmp;
    size = new_size;

    return memo[n - 1];
}
