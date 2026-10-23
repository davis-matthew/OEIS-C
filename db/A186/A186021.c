/*
 * A186021  a(n) = Bell(n)*(2 - 0^n).
 * https://oeis.org/A186021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1           for n = 0
 *           a(n) = 2 · Bell(n) for n > 0
 *
 * Bell(n) is computed with the recurrence
 *   Bell(0) = 1
 *   Bell(n+1) = Σ_{k=0..n} C(n,k) · Bell(k)
 *
 * The implementation stores already-computed Bell numbers in a
 * static array and extends it lazily as larger values are requested.
 * Only integer arithmetic is used; the big‑integer implementation
 * (if any) must provide the usual +, -, *, / operators for `seq_t`.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *bell_cache = NULL;    /* array of Bell numbers 0..size-1   */
static idx_t bell_size = 0;         /* number of elements in bell_cache  */

/* Return a(n) */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Extend cache up to n (inclusive) if required */
    if (n >= bell_size) {
        idx_t new_size = n + 1;
        /* allocate a new array */
        seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_arr) return (seq_t)0; /* allocation failure → 0 */

        /* copy existing values and initialise the rest to 0 */
        if (bell_cache) {
            memcpy(new_arr, bell_cache, bell_size * sizeof(seq_t));
            free(bell_cache);
        }
        memset(new_arr + bell_size, 0,
               (new_size - bell_size) * sizeof(seq_t));

        /* Ensure Bell(0) is defined */
        if (bell_size == 0) {
            new_arr[0] = (seq_t)1;
        }

        /* Compute new Bell numbers up to n */
        for (idx_t i = bell_size; i <= n; ++i) {
            if (i == 0) {
                /* Bell(0) already set */
                continue;
            }
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;           /* C(i-1,0) */
            idx_t m = i - 1;                  /* n for the recurrence */
            for (idx_t k = 0; k <= m; ++k) {
                sum += binom * new_arr[k];
                /* update binom for next k:
                 * C(m,k+1) = C(m,k) * (m-k) / (k+1)
                 */
                if (k < m) {
                    seq_t num = binom * (seq_t)(m - k);
                    binom = num / (seq_t)(k + 1);
                }
            }
            new_arr[i] = sum;
        }

        bell_cache = new_arr;
        bell_size  = new_size;
    }

    /* Return 2 * Bell(n) */
    seq_t result = bell_cache[n] + bell_cache[n];
    return result;
}
