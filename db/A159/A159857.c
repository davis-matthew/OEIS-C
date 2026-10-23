/*
 * A159857  Numerator of Hermite(n, 21/22).
 * https://oeis.org/A159857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159857
   a(0) = 1
   a(1) = 21
   a(n) = 21*a(n-1) + 242*(1-n)*a(n-2)  (n >= 2)
   (Numerator of Hermite(n, 21/22))
*/

#include <stdlib.h>
#include <stdint.h>

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    static idx_t cached = 0;          /* highest index computed so far   */
    static seq_t *buf = NULL;         /* cache of computed values        */
    static size_t buf_cap = 0;        /* allocated capacity              */

    /* Ensure the cache holds at least indices 0..n */
    if (buf == NULL) {
        /* start with capacity 2 to hold a(0) and a(1) */
        buf_cap = (n >= 1) ? 2 : 1;
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        if (buf == NULL) return 0;   /* malloc failure → return 0 */

        buf[0] = 1;
        if (n >= 1) buf[1] = 21;
        cached = (n >= 1) ? 1 : 0;
    } else if (cached < n) {
        /* enlarge if necessary */
        size_t new_cap = buf_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* realloc failure → return 0 */
        buf = tmp;
        buf_cap = new_cap;

        /* Compute new terms */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t term1 = 21 * buf[i - 1];
            seq_t coeff = 242 * ((seq_t)1 - (seq_t)i);
            seq_t term2 = coeff * buf[i - 2];
            buf[i] = term1 + term2;
        }
        cached = n;
    }

    return buf[n];
}
