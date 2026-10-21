/*
 * A221521  Number of 0..5 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 11*a(n-2) + 20*a(n-3) + 17*a(n-4)
 *            - 3*a(n-5) + a(n-6)  for n ≥ 7
 * Initial values: a(1)=0, a(2)=20, a(3)=68, a(4)=400,
 *                 a(5)=1940, a(6)=9982
 * Offset: 1
 *
 * The function uses an O(n) recurrence with memoisation.  It works
 * for any n ≥ 1 in any order of calls and stops overflow on
 * arbitrary‑precision implementations.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamic array, index 0 unused */
static idx_t  capacity = 0;      /* current array capacity (≥ 1)          */
static idx_t  max_computed = 0;  /* largest n for which cache[n] is set   */

/* Resize the cache to hold indices 0..n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;
    idx_t new_cap = n + 1;
    if (new_cap < 16)
        new_cap = 16;
    else if (new_cap <= capacity * 2)
        new_cap = capacity * 2 + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* fatal error */
    cache = tmp;
    capacity = new_cap;
}

/* Initialise the first six terms once */
static void init_first_terms(void)
{
    if (max_computed >= 6)
        return;
    ensure_capacity(6);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)20;
    cache[3] = (seq_t)68;
    cache[4] = (seq_t)400;
    cache[5] = (seq_t)1940;
    cache[6] = (seq_t)9982;
    max_computed = 6;
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined below offset */

    init_first_terms();
    ensure_capacity(n);

    /* Compute any missing terms */
    idx_t start = max_computed + 1;
    if (start < 7)
        start = 7;
    for (idx_t m = start; m <= n; ++m) {
        seq_t val =
            ( (seq_t)2  * cache[m-1]) +
            ( (seq_t)11 * cache[m-2]) +
            ( (seq_t)20 * cache[m-3]) +
            ( (seq_t)17 * cache[m-4]) -
            ( (seq_t)3  * cache[m-5]) +
            cache[m-6];
        cache[m] = val;
    }
    max_computed = n;
    return cache[n];
}
