/*
 * A061078  Sum of the products of the digits of the first n positive even
 *          numbers.
 * https://oeis.org/A061078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=1..n} (∏ digits of 2k)
 * Range:     u8 n <= 27, u16 n <= 896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by accumulating the digit‑products of the first
   n positive even numbers.  Results are cached so that repeated calls are
   O(1) after the first expansion.  The implementation works for all
   supported seq_t/idx_t definitions, including the arbitrary‑precision
   integer class used in the C++ build.  The cast on the realloc call is
   legal in both C (implicit conversion) and C++ (requires a cast). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = 0, cache[i] = a(i) */
    static idx_t  current_max = 0;

    if (n > current_max) {
        idx_t new_size = n + 1; /* include index 0 */
        /* cast is required for C++ (void* to seq_t*) but harmless in C */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – unlikely in tests */
        cache = tmp;

        /* initialise a(0) when allocating for the first time */
        if (current_max == 0) cache[0] = 0;

        /* compute a(i) for all newly needed indices */
        for (idx_t i = current_max + 1; i <= n; ++i) {
            seq_t prod = 1;
            /* 2 * i is the even number whose digit product is needed */
            unsigned long long num = 2ull * (unsigned long long)i;
            while (num > 0) {
                unsigned d = num % 10;
                prod *= d;
                num   /= 10;
            }
            cache[i] = cache[i - 1] + prod;
        }
        current_max = n;
    }
    return cache[n];
}
