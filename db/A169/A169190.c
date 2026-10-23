/*
 * A169190  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 33 * 32^(n-1) for 1 ≤ n ≤ 26 a(27) =
 *            44917272433563877177165448180993403911664 for n ≥ 28: a(n) = 31
 *            * (a(n-1)+…+a(n-26)) - 496 * a(n-27)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies allowed: stdlib.h for malloc/realloc, abort
   (no other helpers are required).                                 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of seq_t and idx_t types – they are supplied by the
   build system.                                                      */
extern size_t _floor_log2_uint64(idx_t);   /* dummy to silence unused warning */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cached values of the sequence */
    static seq_t *buf      = NULL;   /* array of computed terms       */
    static size_t  cap     = 0;      /* current capacity of buf       */
    static idx_t   next    = 0;      /* index of first uncomputed cell */

    /* Grow the buffer if it does not cover the requested index */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0) ? (size_t)n + 1 : cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* Out of memory – terminate */
        }
        buf = tmp;
        cap = new_cap;
    }

    /* Compute terms that have not yet been cached */
    for (idx_t i = next; i <= n; ++i) {
        if (i == 0) {
            buf[0] = 1;
        } else if (i <= 26) {
            /* a(n) = 33 * 32^(n-1)   for 1 ≤ n ≤ 26   */
            seq_t val = 33;
            for (idx_t k = 1; k < i; ++k) {
                val *= 32;
            }
            buf[i] = val;
        } else if (i == 27) {
            /* a(27) is a large constant given as a decimal string.
               Parse it lazily and store the result.                 */
            static const char a27_str[] =
                "44917272433563877177165448180993403911664";
            static bool init_done   = false;
            static seq_t a27_value = 0;
            if (!init_done) {
                a27_value = 0;
                for (const char *p = a27_str; *p; ++p) {
                    a27_value = a27_value * 10 + (seq_t)(*p - '0');
                }
                init_done = true;
            }
            buf[27] = a27_value;
        } else {   /* i ≥ 28 – linear recurrence derived from the GF */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 26; ++k) {
                sum += buf[i - k];
            }
            seq_t term = sum * 31;
            term -= buf[i - 27] * (seq_t)496;
            buf[i] = term;
        }
    }

    next = n + 1;
    return buf[n];
}
