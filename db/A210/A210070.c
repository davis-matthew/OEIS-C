/*
 * A210070  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having one or three distinct clockwise edge differences.
 * https://oeis.org/A210070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210070: 
   a(n) = 9*a(n-1) + 9*a(n-2) - 50*a(n-3) - 54*a(n-4) + 12*a(n-5) + 16*a(n-6)
   with initial values 
   a(1)=33, a(2)=313, a(3)=2908, a(4)=27129, a(5)=252951, a(6)=2358878. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0)
        return 0;

    /* Base values (index 1→6) */
    static const seq_t base[6] = {33, 313, 2908, 27129, 252951, 2358878};

    if (n <= 6)
        return base[n-1];

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t done = 0;          /* number of values already stored in cache */

    /* Ensure enough storage for index n */
    if (capacity < (size_t)n) {
        size_t new_cap = (capacity ? capacity << 1 : 16);
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Store base values if not yet present */
    if (done < 6) {
        size_t upto = 6;
        if (upto > n) upto = n;
        for (size_t i = done; i < upto; ++i)
            cache[i] = base[i];
        done = upto;
    }

    /* Compute further values as needed */
    size_t start = (done >= 6) ? done : 6;
    for (size_t i = start; i < (size_t)n; ++i) {
        seq_t sum_pos = 9 * cache[i-1] + 9 * cache[i-2] + 12 * cache[i-5] + 16 * cache[i-6];
        seq_t sum_neg = 50 * cache[i-3] + 54 * cache[i-4];
        cache[i] = sum_pos - sum_neg;
    }
    if ((size_t)n > done)
        done = (size_t)n;

    return cache[n-1];
}
