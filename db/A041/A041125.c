/*
 * A041125  Denominators of continued fraction convergents to sqrt(71).
 * https://oeis.org/A041125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 6960*a(n-8) - a(n-16) with initial values a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=7, a(4)=54, a(5)=61, a(6)=176, a(7)=413, a(8)=6784,
 *            a(9)=13981, a(10)=34746, a(11)=48727, a(12)=375835,
 *            a(13)=424562, a(14)=1224959, a(15)=2874480
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* minimal table of the first 16 values needed to seed the recurrence */
static const seq_t init_vals[16] = {
    (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)7,
    (seq_t)54, (seq_t)61, (seq_t)176, (seq_t)413,
    (seq_t)6784, (seq_t)13981, (seq_t)34746, (seq_t)48727,
    (seq_t)375835, (seq_t)424562, (seq_t)1224959, (seq_t)2874480
};

seq_t a(idx_t n)
{
    /* trivial cases: n < 16 */
    if (n < 16) {
        return init_vals[(size_t)n];
    }

    size_t m = (size_t)n + 1;            /* number of elements needed */

#if defined(__cplusplus)
    /* C++: use new[] so that class constructors, if any, are called */
    seq_t *arr = new seq_t[m];
#else
    /* C: use malloc, later freed with free() */
    seq_t *arr = (seq_t*)malloc(m * sizeof(seq_t));
    if (!arr) abort();
#endif

    /* copy the seed values */
    for (size_t i = 0; i < 16; ++i) {
        arr[i] = init_vals[i];
    }

    /* compute the remaining terms using the linear recurrence */
    for (size_t i = 16; i < m; ++i) {
        arr[i] = (seq_t)6960 * arr[i - 8] - arr[i - 16];
    }

    seq_t result = arr[n];

#if defined(__cplusplus)
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
