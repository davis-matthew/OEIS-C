/*
 * A137718  A scaled Hankel transform.
 * https://oeis.org/A137718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-4, a(2)=2, a(3)=4; For n>=4: a(n) = -2*a(n-2) -
 *            4*a(n-4) (derived from the given generating function)
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Allocate an array big enough to hold all terms up to n. */
    size_t size = (size_t)(n + 1);
    seq_t *terms = mem_new(seq_t, size);
    if (terms == NULL) return (seq_t)0;   /* unlikely but defensively */

    /* Initialisation of the array. */
    terms[0] = (seq_t)1;
    terms[1] = (seq_t)-4;
    terms[2] = (seq_t)2;
    terms[3] = (seq_t)4;

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        terms[i] = - ((seq_t)2 * terms[i - 2] + (seq_t)4 * terms[i - 4]);
    }

    seq_t result = terms[n];
    mem_free(terms);
    return result;
}
