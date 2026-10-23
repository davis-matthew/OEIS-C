/*
 * A028028  Expansion of 1/((1-3*x)*(1-4*x)*(1-5*x)*(1-9*x)).
 * https://oeis.org/A028028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 155*a(n-2) + 483*a(n-3) - 540*a(n-4) *
 *            a(0)=1, a(1)=21, a(2)=286, a(3)=3234
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms      */
static idx_t  computed = 3;          /* highest index computed so far */
static size_t capacity = 0;          /* current malloc capacity      */

static void ensure_capacity(idx_t n)
{
    if (!cache) {
        /* allocate at least enough for the first four values */
        capacity = (n < 4) ? 4 : (size_t)(n + 1);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)286;
        cache[3] = (seq_t)3234;
        computed = 3;
    } else if ((size_t)(n + 1) > capacity) {
        size_t newcap = capacity;
        while ((size_t)(n + 1) > newcap) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        capacity = newcap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n <= computed) return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prod1 = cache[i - 1] * (seq_t)21;
        seq_t prod2 = cache[i - 2] * (seq_t)155;
        seq_t prod3 = cache[i - 3] * (seq_t)483;
        seq_t prod4 = cache[i - 4] * (seq_t)540;

        /* Overflow is impossible here because every product is < a(i) */
        cache[i] = prod1 - prod2 + prod3 - prod4;
    }
    computed = n;
    return cache[n];
}
