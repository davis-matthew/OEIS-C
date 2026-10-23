/*
 * A041935  Denominators of continued fraction convergents to sqrt(490).
 * https://oeis.org/A041935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041935: Denominators of continued fraction convergents to sqrt(490).
   Formula: a(n) = 2079362*a(n-10) - a(n-20) for n > 19.
   Initial values:
   a(0)=1, a(1)=7, a(2)=15, a(3)=22, a(4)=103, a(5)=434, a(6)=1839, a(7)=2273,
   a(8)=6385, a(9)=46968, a(10)=2072977, a(11)=14557807, a(12)=31188591,
   a(13)=45746398, a(14)=214174183, a(15)=902443130, a(16)=3823946703,
   a(17)=4726389833, a(18)=13276726369, a(19)=97663474416 */
#include <stdlib.h>

/* External function: seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* static cache of computed terms, index 0 .. cached-1 are available */
    static seq_t *cache = NULL;
    static size_t cached = 0;            /* number of stored terms */

    /* Initial 20 terms, const array */
    static const seq_t init[20] = {
        1ULL, 7ULL, 15ULL, 22ULL, 103ULL, 434ULL, 1839ULL, 2273ULL,
        6385ULL, 46968ULL, 2072977ULL, 14557807ULL, 31188591ULL,
        45746398ULL, 214174183ULL, 902443130ULL, 3823946703ULL,
        4726389833ULL, 13276726369ULL, 97663474416ULL
    };
    const seq_t coeff = (seq_t)2079362ULL;   /* recurrence coefficient */

    /* Directly return if n is within the initial block */
    if (n < 20)
        return init[n];

    /* First allocation: store the 20 initial values */
    if (!cache) {
        cached = 20;
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure, unlikely */
        for (size_t i = 0; i < cached; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if we do not yet have term n */
    if ((size_t)n >= cached) {
        size_t new_cap = cached * 2;
        if (new_cap <= (size_t)n)
            new_cap = n + 1;    /* ensure we can hold term n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (size_t i = cached; i < new_cap; ++i) {
            seq_t prod = cache[i - 10] * coeff;
            cache[i] = prod - cache[i - 20];
        }
        cached = new_cap;
    }

    return cache[n];
}
