/*
 * A225132  Convolutory inverse of the Thue Morse sequence.
 * https://oeis.org/A225132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; For n >= 2, a(n) = -\sum_{k=1}^{n-1} a(k) * t(n+1-k),
 *            where t(m) = 1 if the number of 1‑bits in binary(m) is odd,
 *            otherwise t(m) = 0 (the Thue-Morse 0‑1 sequence). This
 *            recurrence produces the convolutory inverse of the Thue-Morse
 *            generating function
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 65, i64 n <= 130,
 *            i128 n <= 260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; index 0 is unused */
    static seq_t *buf = NULL;
    static idx_t computed = 0;          /* highest n computed so far */

    if (n == (idx_t)0)
        return (seq_t)0;                /* not used for valid queries */

    /* Grow the buffer to accommodate index n */
    if (n > computed) {
        /* Allocate space for 0 .. n (index 0 unused) */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;            /* allocation failure */

        buf = tmp;
        /* Initialize new elements as we compute them below */
    }

    /* Ensure a(1) is set */
    if (computed == 0) {
        buf[1] = (seq_t)1;
        computed = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        /* sum over k = 1 .. m-1 of a(k) * t(m+1-k) */
        for (idx_t k = 1; k < m; ++k) {
            /* t(m+1-k) = parity of (m+1-k) */
            if (__builtin_parityll((uint64_t)(m + 1 - k)))
                sum += buf[k];
        }
        buf[m] = -sum;
    }

    computed = n;
    return buf[n];
}
