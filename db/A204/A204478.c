/*
 * A204478  Number of (n+2) X 5 0..3 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A204478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204478 - Number of (n+2)×5 0..3 arrays with every 3×3 subblock having
   exactly three equal elements in a row horizontally, vertically or
   north‑west diagonal.

   Recurrence (n > 13):
     a(n) = 4*a(n-1)+a(n-2)-2*a(n-3)-19*a(n-4)-4*a(n-5)
            +21*a(n-6)+24*a(n-7)+3*a(n-8)-18*a(n-9)-9*a(n-10)

   Initial terms:
     a(1)=1157, a(2)=3147, a(3)=608, a(4)=1587, a(5)=2632, a(6)=9290,
     a(7)=34964, a(8)=124697, a(9)=471260, a(10)=1787757, a(11)=6755022,
     a(12)=25607122, a(13)=97087698 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;     /* storage for computed terms */
static size_t cache_cap = 0;     /* allocated capacity of cache    */
static idx_t  max_computed = 0;  /* largest n for which a(n) is stored */

/* enlarge cache to at least n entries */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with the first 13 terms */
static void init_sequence(void)
{
    const idx_t init_n = 13;
    cache_cap = init_n;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    static const seq_t init_vals[13] = {
        1157, 3147, 608, 1587, 2632, 9290, 34964, 124697, 471260,
        1787757, 6755022, 25607122, 97087698
    };
    for (idx_t i = 0; i < init_n; ++i) cache[i] = init_vals[i];
    max_computed = init_n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) undefined in OEIS */
    if (!cache) init_sequence();

    if (n <= max_computed) return cache[n-1];

    /* recurrence coefficients: a(n‑1), a(n‑2), … , a(n‑10) */
    const int coeffs[10] = {4, 1, -2, -19, -4, 21, 24, 3, -18, -9};

    ensure_capacity(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 10; ++j) {
            seq_t prev = cache[i - j - 2];        /* a(i‑(j+1)) */
            int   c    = coeffs[j];
            seq_t prod = prev * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0) pos += prod;
            else        neg += prod;
        }
        cache[i-1] = pos - neg;                   /* non‑negative result */
    }
    max_computed = n;
    return cache[n-1];
}
