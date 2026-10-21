/*
 * A052566  Expansion of e.g.f. (2 + x)/(1 - x^2).
 * https://oeis.org/A052566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! if n is odd, * a(n) = 2 * n! if n is even. * This
 *            implementation precomputes factorials up to the maximum n
 *            requested * using a static cache. The factorial of 0 is set to
 *            1, so the result for * n = 0 correctly yields 2. The code works
 *            with any seq_t type supported * by the build (fixed‑...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for factorials */
    static seq_t *fact = NULL;       /* array of factorials, fact[i] = i! */
    static size_t size = 0;          /* number of factorials stored (>=1 when allocated) */
    static size_t capacity = 0;      /* allocated array size */

    /* Allocate or grow the factorial array to hold up to n+1 entries */
    if (fact == NULL) {                          /* first call: allocate n+1 slots */
        size = 0;                                /* no factorials yet (but will compute 0!) */
        capacity = n + 1;
        fact = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (fact == NULL) abort();              /* cannot allocate */
        fact[0] = (seq_t)1;                     /* 0! = 1 */
        size = 1;
    } else if (capacity < (size_t)n + 1) {       /* need more space */
        size_t new_cap = capacity * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (fact == NULL) abort();
        capacity = new_cap;
    }

    /* Compute missing factorials up to n */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    size = n + 1;

    /* Calculate the sequence value */
    seq_t result = fact[n];
    if ((n & 1) == 0) {           /* even n: multiply by 2 */
        result *= (seq_t)2;
    }
    return result;
}
