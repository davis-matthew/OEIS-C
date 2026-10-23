/*
 * A104379  a(n) = square root of digital reversal of A102859(n)^2.
 * https://oeis.org/A104379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sqrt( rev( A102859(n)^2 ) )
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   ensure_capacity, is_square, isqrt, rev_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104379: square root of digital reversal of A102859(n)^2. */

#include "../utils/rev_seq.h"        /* rev_seq(seq_t x) */
#include "../utils/is_square.h"      /* is_square(seq_t x) */
#include "../utils/isqrt.h"          /* isqrt(seq_t x) */
#include "../utils/ensure_capacity.h"/* ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap) */
#include <stddef.h>                 /* size_t */

seq_t a(idx_t n)
{
    /* Static state to accumulate the sequence values on demand. */
    static seq_t *values = NULL;   /* array holding a(1), a(2), ... */
    static size_t cap = 0;         /* current allocation capacity */
    static size_t len = 0;         /* number of values currently stored */
    static idx_t next_b = 0;       /* next candidate b = A102859(n) to test */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        seq_t b = (seq_t)next_b;
        seq_t b2 = b * b;                          /* b^2 */
        seq_t rev_b2 = rev_seq(b2);                /* decimal reversal of b^2 */
        if (is_square(rev_b2))
        {
            seq_t a_val = isqrt(rev_b2);            /* sqrt of rev_b2, guaranteed integer */

            /* Ensure capacity for the new value. */
            if (len == cap)
                ensure_capacity(len + 1, &values, &cap);

            values[len++] = a_val;
        }
        ++next_b;
    }

    return values[n - 1];
}
