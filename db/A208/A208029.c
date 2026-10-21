/*
 * A208029  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2) + 2*a(n-3) - 10*a(n-4) for n≥5
 *            Initial terms: a(1)=9, a(2)=81, a(3)=279, a(4)=1377 The function
 *            caches the first 10000 values and uses a rolling window for
 *            larger n. It works for any unsigned idx_t and any seq_t that
 *            supports the usual arithmetic operators
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_LIMIT 10000U

/* Cache for the first CACHE_LIMIT results. */
static seq_t cache[CACHE_LIMIT + 1];
static idx_t max_cached = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (n <= CACHE_LIMIT) {
        /* initialise the first four terms if needed */
        if (max_cached < 1) { cache[1] = (seq_t)9;     max_cached = 1; }
        if (max_cached < 2) { cache[2] = (seq_t)81;    max_cached = 2; }
        if (max_cached < 3) { cache[3] = (seq_t)279;   max_cached = 3; }
        if (max_cached < 4) { cache[4] = (seq_t)1377;  max_cached = 4; }

        if (max_cached < n) {
            for (idx_t i = max_cached + 1; i <= n; ++i) {
                cache[i] = (seq_t)2 * cache[i-1]
                         + (seq_t)11 * cache[i-2]
                         + (seq_t)2 * cache[i-3]
                         - (seq_t)10 * cache[i-4];
            }
            max_cached = n;
        }
        return cache[n];
    }

    /* For n > CACHE_LIMIT use a rolling window; no dynamic allocation. */
    seq_t a4 = (seq_t)9;
    seq_t a3 = (seq_t)81;
    seq_t a2 = (seq_t)279;
    seq_t a1 = (seq_t)1377;
    seq_t a_cur = a1;

    for (idx_t i = 5; i <= n; ++i) {
        a_cur = (seq_t)2 * a1
              + (seq_t)11 * a2
              + (seq_t)2 * a3
              - (seq_t)10 * a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = a_cur;
    }
    return a_cur;
}
