/*
 * A210136  Number of (n+1) X 2 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having one, two or four distinct values, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A210136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 111*a(n-2) + 266*a(n-3) - 180*a(n-4) -
 *            24*a(n-5) + 32*a(n-6). Initial values: a(1)=1, a(2)=10,
 *            a(3)=105, a(4)=1018, a(5)=9157, a(6)=77934
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] unused */
static idx_t cache_len = 0;        /* current number of stored elements (>=0) */
static idx_t cache_cap = 0;        /* current allocated capacity */

static void ensure_cache_cap(idx_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);  /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

static void init_base_values(void) {
    /* size at least 7 (indices 0..6) */
    if (cache_len >= 7) return;
    ensure_cache_cap(7);
    cache[0] = 0;                    /* unused */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)105;
    cache[4] = (seq_t)1018;
    cache[5] = (seq_t)9157;
    cache[6] = (seq_t)77934;
    cache_len = 7;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */
    init_base_values();
    if (n < cache_len) return cache[n];
    ensure_cache_cap(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t p1 = (seq_t)18   * cache[i-1];
        seq_t p2 = (seq_t)266  * cache[i-3];
        seq_t p3 = (seq_t)32   * cache[i-6];
        seq_t pos = p1 + p2 + p3;              /* sum of positive terms */

        seq_t n1 = (seq_t)111  * cache[i-2];
        seq_t n2 = (seq_t)180  * cache[i-4];
        seq_t n3 = (seq_t)24   * cache[i-5];
        seq_t neg = n1 + n2 + n3;              /* sum of negative terms */

        cache[i] = pos - neg;                 /* result is positive */
    }
    cache_len = n + 1;
    return cache[n];
}
