/*
 * A025278  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          with initial values 1,-1,1,1.
 * https://oeis.org/A025278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(2) = -1, a(3) = 1, a(4) = 1, a(5) = 0 For n >= 6:
 *            n*a(n) = 2*(2*n-3)*a(n-1) - 8*(n-3)*a(n-2) + 6*(2*n-9)*a(n-3) -
 *            8*(n-6)*a(n-4) (recurrence valid for n >= 6)
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 36, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* maximum number of terms that can be computed (adjust if larger values are required) */
#define A025278_MAX_N 200000

/* static cache of computed terms */
static seq_t a_cache[A025278_MAX_N + 1];
static idx_t cache_size = 0;   /* highest index that has been computed */

/* initialise the cache with the first five terms */
static void init_cache(void)
{
    if (cache_size != 0)
        return;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)-1;
    a_cache[3] = (seq_t)1;
    a_cache[4] = (seq_t)1;
    a_cache[5] = (seq_t)0;
    cache_size = 5;
}

/* extend the cache up to at least n (must be <= A025278_MAX_N) */
static void extend_cache(idx_t n)
{
    if (n <= cache_size || n > A025278_MAX_N)
        return;

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 5) {
            /* value already set in init_cache */
            continue;
        }
        /* recurrence for i >= 6 */
        if (i >= 6) {
            seq_t num =
                ((seq_t)2 * (seq_t)(2 * i - 3)) * a_cache[i - 1]
              - ((seq_t)8 * (seq_t)(i - 3)) * a_cache[i - 2]
              + ((seq_t)6 * (seq_t)(2 * i - 9)) * a_cache[i - 3]
              - ((seq_t)8 * (seq_t)(i - 6)) * a_cache[i - 4];
            a_cache[i] = num / (seq_t)i;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    init_cache();                 /* ensure first five terms are present */

    if (n > A025278_MAX_N)
        abort();                  /* out of bounds: should not happen for valid tests */

    extend_cache(n);              /* compute up to n if necessary */
    return a_cache[n];
}
