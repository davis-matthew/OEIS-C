/*
 * A232622  Number of incongruent domino tilings of the 3 X (2n) board.
 * https://oeis.org/A232622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)-a(n-2)-19*a(n-3)+19*a(n-4)+a(n-5)-5*a(n-6)+a(n-7) with
 *            a(0)=1,a(1)=2,a(2)=5,a(3)=14,a(4)=46,a(5)=156,a(6)=561
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of previously computed terms */
    static size_t cap   = 0;      /* current allocated capacity */
    static size_t cache_len = 0;  /* number of initialized entries   */

    /* Ensure we have space for indices up to n */
    if (n >= (idx_t)cap) {
        size_t newcap = (cap == 0) ? 8 : cap;
        while (newcap <= (size_t)n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)46;
        cache[5] = (seq_t)156;
        cache[6] = (seq_t)561;
        cache_len = 7;
    }

    if (n < (idx_t)cache_len)
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 5 * cache[i-1];
        val -= cache[i-2];
        val -= 19 * cache[i-3];
        val += 19 * cache[i-4];
        val += cache[i-5];
        val -= 5 * cache[i-6];
        val += cache[i-7];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
