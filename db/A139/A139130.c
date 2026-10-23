/*
 * A139130  a(n) = Sum_{k=1..n} d(d(k)), where d(k) = number of divisors of k.
 * https://oeis.org/A139130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} d(d(k)), where d(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not required by OEIS offset */

    /* ---------- divisor counts for 1..n ----------
       Static storage so repeated calls reuse the array.  */
    static idx_t *d = NULL;
    static idx_t d_size = 0;   /* number of elements allocated in d */

    if (n + 1 > (idx_t)d_size) {
        idx_t *tmp = NULL;
        if (d == NULL) {
            tmp = (idx_t*)malloc(((size_t)(n + 1)) * sizeof(idx_t));
        } else {
            tmp = (idx_t*)realloc(d, ((size_t)(n + 1)) * sizeof(idx_t));
        }
        if (tmp == NULL) return 0;   /* allocation failed – return zero */
        d = tmp;
        d_size = n + 1;
    }

    /* initialise to zero */
    for (idx_t i = 1; i <= n; ++i) d[i] = 0;

    /* sieve divisor counts */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            d[j] += 1;
        }
    }

    /* find the maximum divisor count among 1..n */
    idx_t max_d = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (d[i] > max_d) max_d = d[i];
    }

    /* ---------- divisor counts for numbers up to max_d ----------
       Another static array for repeated calls.  */
    static idx_t *d2 = NULL;
    static idx_t d2_size = 0;    /* number of elements allocated in d2 */

    if (max_d + 1 > (idx_t)d2_size) {
        idx_t *tmp2 = NULL;
        if (d2 == NULL) {
            tmp2 = (idx_t*)malloc(((size_t)(max_d + 1)) * sizeof(idx_t));
        } else {
            tmp2 = (idx_t*)realloc(d2, ((size_t)(max_d + 1)) * sizeof(idx_t));
        }
        if (tmp2 == NULL) return 0;
        d2 = tmp2;
        d2_size = max_d + 1;
    }

    /* initialise to zero */
    for (idx_t i = 1; i <= max_d; ++i) d2[i] = 0;

    /* sieve divisor counts up to max_d */
    for (idx_t i = 1; i <= max_d; ++i) {
        for (idx_t j = i; j <= max_d; j += i) {
            d2[j] += 1;
        }
    }

    /* ---------- compute the sum ----------
       The result fits in seq_t for the test cases where seq_t is 64‑bit,
       and is arbitrary‑precision when the compiler provides it.  */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += (seq_t)d2[d[i]];
    }
    return sum;
}
