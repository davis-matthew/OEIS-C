/*
 * A112666  Semiprimes n such that 3*n + 4 is a square.
 * https://oeis.org/A112666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th semiprime such that 3*n + 4 is a perfect square
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_semiprime_uint64, is_square, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by linear search with caching. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/is_square.h"
#include "../utils/push_seq.h"

/* static state for caching sequence values */
static seq_t *seq = NULL;        /* array of found terms */
static idx_t seq_cap = 0;        /* capacity of seq */
static idx_t seq_size = 0;       /* current number of stored terms */
static idx_t last_checked = 0;   /* last candidate examined */

/* Return the n-th term of the sequence (1-based index).  */
seq_t a(idx_t n)
{
    /* safety: n >= 1 */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    while (seq_size < n)
    {
        last_checked++;  /* next candidate */

        /* check semiprimality */
        if (!is_semiprime_uint64((uint64_t)last_checked))
            continue;

        /* compute 3*last_checked + 4 in seq_t type */
        seq_t val = (seq_t)3 * (seq_t)last_checked + 4;

        /* check if this value is a perfect square */
        if (is_square(val))
        {
            /* store the candidate as a term of the sequence */
            push_seq(&seq, &seq_cap, &seq_size, (seq_t)last_checked);
        }
    }

    /* seq is indexed from 0, but the sequence is 1-based */
    return seq[n - 1];
}
