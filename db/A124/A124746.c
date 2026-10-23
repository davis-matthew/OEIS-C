/*
 * A124746  Expansion of (1+x^2)/(1-x^4+x^5).
 * https://oeis.org/A124746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = coefficient of x^n in (1 + x^2)/(1 - x^4 + x^5)
 * Range:     i8 n <= 37, i16 n <= 73, i32 n <= 145, i64 n <= 288,
 *            i128 n <= 575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The generating function satisfies the recurrence
 *   a(n) = a(n-4) - a(n-5)          for n >= 5
 * with initial values a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1. */

#include <stddef.h>   /* for size_t */

/* The types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n) {
    /* memoization buffer: enough for n up to 10000, per requirements. */
    static seq_t cache[10001];
    static idx_t computed = 5;             /* indices <5 are already stored */
    static int initialized = 0;

    if (!initialized) {                    /* initialise base values once */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 0;
        cache[4] = 1;
        initialized = 1;
    }

    if (n < 5) {                           /* immediately return known cases */
        return cache[n];
    }

    if (n >= computed) {                  /* extend the cache up to n */
        for (idx_t i = computed; i <= n; ++i) {
            /* recurrence: a(i) = a(i-4) - a(i-5)   (rhs is zero for i>=5) */
            cache[i] = cache[i - 4] - cache[i - 5];
        }
        computed = n + 1;
    }

    return cache[n];
}
