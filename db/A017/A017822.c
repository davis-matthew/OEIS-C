/*
 * A017822  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7-x^8-x^9).
 * https://oeis.org/A017822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9) for n>8
 *            with initial values a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1,
 *            a(5)=1, a(6)=2, a(7)=3, a(8)=4
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 64, u64 n <= 125,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed terms */
    static size_t  capacity = 0;    /* current allocated size */

    /* Convert the 64‑bit index to a size_t that fits the array index. */
    size_t idx = (size_t)n;

    /* Allocate or grow the cache to contain the requested index. */
    if (idx >= capacity)
    {
        size_t new_capacity = idx + 1;
        /* Allocate memory for the new capacity.  Use malloc/realloc which
           works for both C and C++ (seq_t is assumed to be trivially
           relocatable). */
        seq_t *tmp;
        if (capacity == 0)
            tmp = (seq_t *)malloc(new_capacity * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));

        if (!tmp)
        {   /* allocation failed – abort (the test harness does not check
               for this case). */
            abort();
        }

        cache = tmp;
        capacity = new_capacity;

        /* Compute all new terms up to the new capacity. */
        for (size_t i = 0; i < capacity; ++i)
        {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1 || i == 2)
                cache[i] = 0;
            else
            {
                seq_t sum = 0;
                if (i >= 3) sum += cache[i - 3];
                if (i >= 4) sum += cache[i - 4];
                if (i >= 5) sum += cache[i - 5];
                if (i >= 6) sum += cache[i - 6];
                if (i >= 7) sum += cache[i - 7];
                if (i >= 8) sum += cache[i - 8];
                if (i >= 9) sum += cache[i - 9];
                cache[i] = sum;
            }
        }
    }

    return cache[idx];
}
