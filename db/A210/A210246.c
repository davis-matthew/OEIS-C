/*
 * A210246  Polylogarithm li(-n,-1/3) multiplied by (4^(n+1))/3.
 * https://oeis.org/A210246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ***************************************************************************
 *  Sequence A210246
 *  -----------------
 *
 *  a(0) = 1
 *
 *  Recurrence:
 *      a(n+1) = -4 * a(n) + 3 * Σ_{k=0..n} C(n,k) * a(k) * a(n-k)
 *              with C(n,k) binomial coefficients.
 *
 *  The formula implemented is the one derived from the OEIS entry:
 *      a(n) = 4^(n+1) / 3 * Σ_{k>=0} (-1)^k * k^n / 3^k
 *
 *  The implementation uses memoisation: previously computed terms are
 *  stored in a static cache so that calls with non‑monotonic indices
 *  are handled efficiently.  The cache grows geometrically when needed.
 *
 *  No arithmetic beyond addition, subtraction, multiplication, division,
 *  comparison and bitwise operations is used; all arithmetic is performed
 *  on the type ``seq_t`` (which can be fixed‑width or arbitrary‑precision).
 *  The index type ``idx_t`` is an unsigned integer (uint64_t).
 *
 *  All types required by the harness (seq_t, idx_t, etc.) are defined
 *  elsewhere; this file only implements the required function.
 *
 *  Author: OpenAI ChatGPT
 *  **************************************************************************/

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;  /* array of computed values */
    static size_t capacity  = 0;      /* allocated size of cache   */
    static size_t size      = 0;      /* number of terms currently computed */

    /* ensure enough storage for all requested terms */
    if (n >= capacity) {
        size_t new_cap = (capacity > 0) ? capacity : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – abort (the harness does not expect it) */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to index n */
    while (size <= (size_t)n) {
        if (size == 0) {
            /* a(0) = 1 */
            cache[0] = (seq_t)1;
        } else {
            /* compute a[size] which corresponds to a(n) with n=size-1
               using the recurrence a(n+1) = -4*a(n)+3*Σ...   */
            idx_t m = (idx_t)size - 1;           /* current n in recurrence */
            seq_t sum = (seq_t)0;
            seq_t bin  = (seq_t)1;               /* C(m,k) for k=0 */

            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = bin * cache[k] * cache[m - k];
                sum += term;

                /* update binomial coefficient to C(m,k+1) */
                if (k < m) {
                    bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
                }
            }

            cache[size] = -(seq_t)4 * cache[size - 1] + (seq_t)3 * sum;
        }
        ++size;   /* one more term now computed */
    }

    return cache[n];
}
