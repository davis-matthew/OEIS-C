/*
 * A071542  Number of steps to reach 0 starting with n and using the iterated
 *          process : x -> x - (number of 1's in binary representation of x).
 * https://oeis.org/A071542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1171, u16 n <= 9306, u32 n <= 9306, u64 n <= 9306,
 *            u128 n <= 9306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(n)=1+a(n-popcount(n)) for n>0
   (popcount = number of 1's in binary representation of n) */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of computed values   */
static idx_t  computed = 0;   /* number of entries currently computed (>=1 when cache[0] is valid) */
static idx_t  allocated = 0;  /* capacity of the cache array */

static void ensure_computed(idx_t n) {
    /* guarantee that cache[n] exists and is correct */

    /* handle the very first call */
    if (computed == 0) {
        if (allocated == 0) {
            allocated = 1;
            cache = (seq_t *)malloc(allocated * sizeof(seq_t));
            if (!cache) abort();           /* allocation failed */
        }
        cache[0] = 0;                      /* a(0)=0 */
        computed = 1;
    }

    /* expand allocated memory if necessary */
    if (n + 1 > allocated) {
        idx_t new_alloc = allocated ? allocated << 1 : 1;
        while (new_alloc <= n) new_alloc <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        allocated = new_alloc;
    }

    /* fill missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;  /* should not happen for i>=1, but keep safety */
            continue;
        }
        idx_t sub = i - (__builtin_popcountll((unsigned long long)i));
        /* sub is always < i, thus already computed */
        cache[i] = cache[sub] + 1;
    }
    computed = n + 1;
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return cache[n];
}
