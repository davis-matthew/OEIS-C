/*
 * A061994  Number of ways to place 4 nonattacking queens on an n X n board.
 * https://oeis.org/A061994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 26, u64 n <= 383, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + a(n-2) - 9*a(n-3) + 12*a(n-5) + 7*a(n-6)
 *            - 15*a(n-7) - 16*a(n-8) + 16*a(n-9) + 15*a(n-10)
 *            - 7*a(n-11) - 12*a(n-12) + 9*a(n-14) - a(n-15) - 3*a(n-16) + a(n-17)
 * Initial values:
 * a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 * a(4)=2,  a(5)=82,   a(6)=982,   a(7)=7002,
 * a(8)=34568, a(9)=131248, a(10)=412596, a(11)=1123832,
 * a(12)=2739386, a(13)=6106214, a(14)=12654614,
 * a(15)=24675650, a(16)=45704724, a(17)=80999104
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* holds a(0) … a(max) */
static size_t cached_size = 0;       /* number of elements currently stored  */

/* initialise cache with the first 18 terms */
static void init_cache(void) {
    const seq_t init[18] = {
        (seq_t)0ULL, (seq_t)0ULL, (seq_t)0ULL, (seq_t)0ULL,
        (seq_t)2ULL, (seq_t)82ULL, (seq_t)982ULL, (seq_t)7002ULL,
        (seq_t)34568ULL, (seq_t)131248ULL, (seq_t)412596ULL,
        (seq_t)1123832ULL, (seq_t)2739386ULL, (seq_t)6106214ULL,
        (seq_t)12654614ULL, (seq_t)24675650ULL,
        (seq_t)45704724ULL, (seq_t)80999104ULL
    };
    size_t i;
    cache = (seq_t *)malloc(18 * sizeof(seq_t));
    if (!cache) exit(1);               /* allocation failure – abort */
    for (i = 0; i < 18; ++i) cache[i] = init[i];
    cached_size = 18;
}

/* ensure that cache contains values up to at least idx (inclusive) */
static void grow_cache(idx_t idx) {
    size_t required = (size_t)idx + 1;
    if (required <= cached_size) return;

    seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!tmp) exit(1);                  /* allocation failure – abort */
    cache = tmp;

    /* compute new entries using the recurrence   */
    size_t i;
    for (i = cached_size; i < required; ++i) {
        /* all indices i-17 … i-1 are valid because i >= 17 */
        seq_t pos = 0, neg = 0;

        /* positive contributions */
        pos += 3 * cache[i-1];
        pos += cache[i-2];
        pos += 12 * cache[i-5];
        pos += 7  * cache[i-6];
        pos += 16 * cache[i-9];
        pos += 15 * cache[i-10];
        pos += 9  * cache[i-14];
        pos += cache[i-17];

        /* negative contributions */
        neg += 9  * cache[i-3];
        neg += 15 * cache[i-7];
        neg += 16 * cache[i-8];
        neg += 7  * cache[i-11];
        neg += 12 * cache[i-12];
        neg += cache[i-15];
        neg += 3  * cache[i-16];

        /* all additions are non‑negative and the positive
         * sum is guaranteed to exceed the negative sum for
         * n ≥ 17, so unsigned subtraction yields the correct
         * non‑negative result. */
        cache[i] = pos - neg;
    }
    cached_size = required;
}

seq_t a(idx_t n) {
    if (!cache) init_cache();          /* first call – initialise */
    size_t idx = (size_t)n;
    if (idx >= cached_size) grow_cache(n);
    return cache[idx];
}
