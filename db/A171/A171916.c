/*
 * A171916  Van Eck's sequence (cf. A181391) starting with a(1) = 6.
 * https://oeis.org/A171916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=6; for n>1, a(n)=n₋prev if the value a(n‑1) already
 *            appeared earlier at index prev; otherwise a(n)=0
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamic storage for the sequence */
static seq_t *seq = NULL;
static size_t seq_cap = 0;   /* allocated capacity */
static size_t seq_len = 0;   /* number of terms already stored */

/* Ensure the array has room for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Compute the n-th term of Van Eck's sequence with a(1)=6. */
seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;  /* out of bounds – not part of the offset */

    /* first call: initialise with a(1) */
    if (seq_len == 0u) {
        ensure_seq_cap(1u);
        seq[0] = (seq_t)6;
        seq_len = 1u;
    }

    /* generate terms up to n */
    while (seq_len < (size_t)n) {
        /* current term a(seq_len) (1‑based index seq_len) */
        seq_t cur = seq[seq_len - 1];
        idx_t prev_idx = 0u;   /* 1‑based index of previous occurrence */
        if (seq_len > 1u) {
            /* scan backwards for a previous occurrence of cur */
            size_t i = seq_len - 2u;   /* zero‑based index of element before cur */
            for (;;) {
                if (seq[i] == cur) {
                    prev_idx = (idx_t)(i + 1u);  /* convert to 1‑based */
                    break;
                }
                if (i == 0u)
                    break;               /* reached start of array */
                --i;
            }
        }

        /* distance from the current index to the previous occurrence */
        seq_t next_val = (prev_idx == 0u)
                         ? (seq_t)0
                         : (seq_t)(seq_len - prev_idx);   /* seq_len = current n */

        ensure_seq_cap(seq_len + 1u);
        seq[seq_len] = next_val;
        ++seq_len;
    }

    return seq[n - 1];  /* zero‑based array index */
}
