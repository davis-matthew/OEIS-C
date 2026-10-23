/*
 * A178743  a(n) = A000041(n) mod 10.
 * https://oeis.org/A178743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000041(n) mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partition function modulo 10 using Euler's recurrence
   with generalized pentagonal numbers.  The values are cached in a
   static array that grows as needed.  The implementation handles
   ordinary unsigned integer types as well as arbitrary‑precision
   integer classes (compiled as C++17). */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>   /* for EXIT_FAILURE */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* In C++ the array elements are objects that require proper
       construction and destruction.  We use operator new[] / delete[]
       for allocation. */
    static seq_t *p = nullptr;
    static idx_t sz = 0;
#else
    /* In C the array elements are plain integers.  realloc works fine. */
    static seq_t *p = NULL;
    static idx_t sz = 0;
#endif

    /* Grow the cache if necessary. */
    if (n >= sz) {
        idx_t new_sz = n + 1;

#ifdef __cplusplus
        /* Allocate a new array and copy existing values. */
        seq_t *newp = new seq_t[new_sz];
        if (p) {
            for (idx_t i = 0; i < sz; ++i)
                newp[i] = p[i];           /* copy construct */
            delete[] p;                    /* destroy old objects */
        }
        p = newp;
#else
        /* Reallocate a C array. */
        seq_t *tmp = realloc(p, new_sz * sizeof(seq_t));
        if (!tmp) {
            fprintf(stderr, "out of memory in a()\n");
            exit(EXIT_FAILURE);
        }
        p = tmp;
#endif

        /* Compute partition numbers modulo 10 for new indices. */
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i == 0) {                 /* p(0) = 1 */
                p[0] = (seq_t)(1 % 10);
                continue;
            }

            int sum = 0;                  /* temporary sum, int is enough */
            for (idx_t k = 1; ; ++k) {
                /* Generalized pentagonal numbers */
                idx_t pent1 = k * (3 * k - 1) / 2;
                idx_t pent2 = k * (3 * k + 1) / 2;

                int sign = (k & 1) ? 1 : -1;    /* (-1)^(k-1) */

                if (pent1 > i && pent2 > i)
                    break;

                if (pent1 <= i)
                    sum += sign * (int)p[i - pent1];
                if (pent2 <= i)
                    sum += sign * (int)p[i - pent2];
            }

            int mod = sum % 10;
            if (mod < 0)
                mod += 10;
            p[i] = (seq_t)mod;
        }

        sz = new_sz;
    }

    return p[n];
}
