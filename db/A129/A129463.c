/*
 * A129463  Row sums of triangle A129462 (v=2 member of a certain family).
 * https://oeis.org/A129463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(n-2)*(n-1)*a(n-1) - (n-3)^2*(n-1)^2*a(n-2) Initial
 *            values: a(0)=1, a(1)=0, a(2)=-1 (OEIS A129463 – row sums of
 *            A129462)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 13, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* We keep a statically sized cache large enough for the intended
   range of n (about 10000).  For any larger n the function will
   compute directly without memoisation.  This keeps the code
   C11‑ and C++17‑compatible without relying on object constructors
   for an arbitrary‑precision type. */
#define CACHE_SIZE 200000

static seq_t cache[CACHE_SIZE];
static idx_t cache_max = 0;    /* highest pre‑computed index (>=2) */

/* Compute a(n) recursively using the given recurrence.
   Intermediate results are kept in type seq_t. */
seq_t a(idx_t n)
{
    /* Initialise base values once */
    if (cache_max == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-1;
        cache_max = 2;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* For n larger than CACHE_SIZE we fall back to direct
       computation without storing intermediate results. */
    if (n >= CACHE_SIZE) {
        seq_t a_nm1 = a(n-1);
        seq_t a_nm2 = a(n-2);
        seq_t term1 = a_nm1 * (seq_t)2 * (seq_t)(n-2) * (seq_t)(n-1);
        seq_t term2 = a_nm2 * (seq_t)(n-3) * (seq_t)(n-3) *
                      (seq_t)(n-1) * (seq_t)(n-1);
        return term1 - term2;
    }

    /* Fill the cache up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term1 = cache[i-1];
        term1 *= (seq_t)2;
        term1 *= (seq_t)(i-2);
        term1 *= (seq_t)(i-1);

        seq_t term2 = cache[i-2];
        term2 *= (seq_t)(i-3);
        term2 *= (seq_t)(i-3);
        term2 *= (seq_t)(i-1);
        term2 *= (seq_t)(i-1);

        cache[i] = term1 - term2;
    }
    cache_max = n;
    return cache[n];
}
