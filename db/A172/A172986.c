/*
 * A172986  a(0) = 0, a(n) = A029826(n+1) for n <= 20, otherwise a(n) = a(n -
 *          1 - n % 20) + A029826(2 + n % 20 ).
 * https://oeis.org/A172986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, less
 * Formula:   not stated
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A172986
 *  a(0) = 0
 *  a(n) = A029826(n+1)          for 1 <= n <= 20
 *  a(n) = a(n-1-n%20) + A029826(2+n%20)   for n > 20
 *
 *  The table A029826(1..21) is taken from the OEIS definition of A029826.
 */

#include <stdlib.h>

static const seq_t A029826_table[22] =
{
    0,    /* unused, placeholder for index 0 */
    0,    /* A029826(1) – unused in A172986 */
    1,    /* A029826(2)  */
   -1,    /* A029826(3)  */
    1,    /* A029826(4)  */
    0,    /* A029826(5)  */
    0,    /* A029826(6)  */
    1,    /* A029826(7)  */
    0,    /* A029826(8)  */
    1,    /* A029826(9)  */
    0,    /* A029826(10) */
    1,    /* A029826(11) */
    1,    /* A029826(12) */
    1,    /* A029826(13) */
    1,    /* A029826(14) */
    2,    /* A029826(15) */
    1,    /* A029826(16) */
    2,    /* A029826(17) */
    3,    /* A029826(18) */
    2,    /* A029826(19) */
    4,    /* A029826(20) */
    3     /* A029826(21) */
};

static const seq_t add_table[20] =
{
    1,  /* A029826(2)   */
   -1,  /* A029826(3)   */
    1,  /* A029826(4)   */
    0,  /* A029826(5)   */
    0,  /* A029826(6)   */
    1,  /* A029826(7)   */
    0,  /* A029826(8)   */
    1,  /* A029826(9)   */
    0,  /* A029826(10)  */
    1,  /* A029826(11)  */
    1,  /* A029826(12)  */
    1,  /* A029826(13)  */
    1,  /* A029826(14)  */
    2,  /* A029826(15)  */
    1,  /* A029826(16)  */
    2,  /* A029826(17)  */
    3,  /* A029826(18)  */
    2,  /* A029826(19)  */
    4,  /* A029826(20)  */
    3   /* A029826(21)  */
};

static seq_t *cache = NULL;          /* dynamic array of computed a(n) values */
static size_t cache_len = 0;         /* number of initialized entries */
static size_t cache_cap = 0;         /* capacity of cache array */

static void ensure_cache_cap(idx_t n)
{
    if (n < cache_cap)
        return;                       /* already enough */
    size_t cap = cache_cap ? cache_cap : 1;
    while (cap <= (size_t)n)
        cap <<= 1;                     /* double until enough */
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* a(0) = 0 by definition   */
        return (seq_t)0;

    /* provide capacity for index n */
    ensure_cache_cap(n);

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* initialise cache[0] if first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* a(0) = 0 */
        cache_len = 1;
    }

    /* compute values sequentially up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 20) {                /* use direct A029826 values   */
            cache[i] = A029826_table[i + 1];   /* A029826(n+1) */
        } else {                      /* recurrence for i > 20       */
            idx_t r     = i % 20;     /* n mod 20                     */
            idx_t prev  = i - 1 - r;  /* n - 1 - n%20                 */
            cache[i] = cache[prev] + add_table[r];
        }
    }
    cache_len = (size_t)n + 1;        /* update length after fill     */
    return cache[n];
}
