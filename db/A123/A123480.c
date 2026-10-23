/*
 * A123480  Coefficients of the series giving the best rational approximations
 *          to sqrt(3).
 * https://oeis.org/A123480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 15*a(n+2) - 15*a(n+1) + a(n) with initial values
 *            a(1)=4, a(2)=60, a(3)=840
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation caches previously-computed terms to allow
   arbitrary ordering of calls.  It only uses integer arithmetic on
   seq_t, so it is valid for both native integer sizes and the
   arbitrary-precision seq_t class. */

#include <stdlib.h>

static seq_t *cache = NULL;            /* array of computed terms      */
static size_t cache_cap = 0;           /* allocated capacity (in terms)*/
static size_t cache_len = 0;           /* number of terms currently stored */

static void ensure_cache(idx_t n)
{
    /* Guarantee that cache[n-1] exists and holds a(n). */
    if (n == 0) return;               /* n < 1 is undefined; ignore */

    /* First allocation (or reallocation if too small). */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? ((n > 16) ? n : 16) : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {            /* allocation failure - abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first three terms if we have just allocated the
       array. */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;              /* a(1) */
        cache[1] = (seq_t)60;             /* a(2) */
        cache[2] = (seq_t)840;            /* a(3) */
        cache_len = 3;
    }

    /* Compute any missing terms up to n. */
    while (cache_len < (size_t)n) {
        seq_t a = cache[cache_len-1];          /* a(k)   */
        seq_t b = cache[cache_len-2];          /* a(k-1) */
        seq_t c = cache[cache_len-3];          /* a(k-2) */
        seq_t term = (seq_t)15 * a - (seq_t)15 * b + c;
        cache[cache_len] = term;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, undefined behaviour */
    ensure_cache(n);
    return cache[n-1];
}
