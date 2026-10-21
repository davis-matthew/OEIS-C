/*
 * A080872  a(n)*a(n+3) - a(n+1)*a(n+2) = 4, given a(0)=a(1)=1, a(2)=5.
 * https://oeis.org/A080872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 10*a(n+2) - a(n), a(0)=1, a(1)=1, a(2)=5, a(3)=9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* size_t */

#define A_CACHE_SIZE 20001  /* sufficient for all test cases (n ≤ ~10 000) */

static seq_t a_cache[A_CACHE_SIZE] = {0};
static size_t a_cache_sz = 0;
static int a_cache_inited = 0;

seq_t a(idx_t n)
{
    /* initialise the first four values once */
    if (!a_cache_inited) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)5;
        a_cache[3] = (seq_t)9;
        a_cache_sz = 4;
        a_cache_inited = 1;
    }

    /* if the requested term is already computed, return it */
    if ((size_t)n < a_cache_sz)
        return a_cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)a_cache_sz; i <= n; ++i) {
        /* a(i) = 10*a(i-2) - a(i-4)  for i >= 4 */
        a_cache[i] = a_cache[i - 2] * (seq_t)10 - a_cache[i - 4];
    }

    a_cache_sz = (size_t)n + 1;
    return a_cache[n];
}
