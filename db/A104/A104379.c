/*
 * A104379  a(n) = square root of digital reversal of A102859(n)^2.
 * https://oeis.org/A104379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sqrt( rev( A102859(n)^2 ) )
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_square, isqrt_seq, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104379: square root of digital reversal of A102859(n)^2. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_square.h"
#include "../utils/isqrt_seq.h"
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

/* Grow an array of seq_t elements to at least min_cap elements. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (min_cap <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1024;
    if (new_cap < min_cap) new_cap = min_cap;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}
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
        seq_t rev_b2 = reverse_digits(b2);                /* decimal reversal of b^2 */
        if (is_square(rev_b2))
        {
            seq_t a_val = isqrt_seq(rev_b2);            /* sqrt of rev_b2, guaranteed integer */

            /* Ensure capacity for the new value. */
            if (len == cap)
                ensure_capacity(len + 1, &values, &cap);

            values[len++] = a_val;
        }
        ++next_b;
    }

    return values[n - 1];
}
