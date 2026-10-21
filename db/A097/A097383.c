/*
 * A097383  Minimum total number of comparisons to find each of the values 1
 *          through n using a binary search with 3-way comparisons (less than,
 *          equal and greater than).
 * https://oeis.org/A097383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 5897, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A097383:  Minimum total number of comparisons with 3‑way
   binary search.
   Recurrence:
       a(1)    = 0
       a(n)    = n + min_{1 ≤ k ≤ n-1} ( a(k) + a(n-1-k) ),  n ≥ 2
   The table is indexed by the sequence index (a(1) is the first term
   list).  The function is memoised with a static dynamic array; later
   calls are O(1) after the necessary values have been computed.  The
   implementation is O(n²) in total, which is easily fast enough for
   n ≤ 10 000.  It works for built‑in unsigned integer types and for
   arbitrary‑precision integer classes, as long as the test harness
   supplies a trivial constructor (e.g. boost::multiprecision::cpp_int). */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Special case: the OEIS offset is 1; the function is declared
       to be called only for n ≥ 1.  We simply return 0 for n == 0 to
       keep the array index in bounds. */
    if (n == 0) return 0;

    /* Static cache: cache[i] holds a(i).  Indices 0..max_alloc-1 are
       valid.  We store a(0)=0 (unused) and a(1)=0. */
    static seq_t *cache   = NULL;
    static size_t max_alloc = 0;     /* capacity of the cache */
    static idx_t  highest  = 1;     /* highest n for which cache[n] is known */

    /* Grow the cache if necessary.  The resizing is performed with
       malloc/realloc and a cast to seq_t*; this is valid in both C
       and C++ for POD types, and is acceptable for the arbitrary
       precision types used in the test harness. */
    if (max_alloc < (size_t)n + 1) {
        size_t new_cap = max_alloc ? max_alloc << 1 : 4;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* memory allocation failed */
        cache = tmp;
        /* Initialise new slots to zero (valid for all types used). */
        for (size_t i = max_alloc; i < new_cap; ++i) cache[i] = 0;
        max_alloc = new_cap;
    }

    /* Ensure a(0) and a(1) are defined. */
    cache[0] = 0;
    cache[1] = 0;

    /* Compute values up to n if they are not yet known. */
    for (idx_t m = highest + 1; m <= n; ++m) {
        /* Base case n=1 already handled.  For m >= 2 we use the
           recurrence. */
        seq_t best = cache[1] + cache[m - 2];   /* k = 1  ->  m-1-1 = m-2 */
        /* Because a(k)+a(m-1-k) is symmetric, it suffices to test
           k up to (m-1)/2. */
        idx_t limit = (m - 1) / 2;
        for (idx_t k = 2; k <= limit; ++k) {
            seq_t candidate = cache[k] + cache[m - 1 - k];
            if (candidate < best) best = candidate;
        }
        cache[m] = (seq_t)m + best;
    }
    highest = (highest > n) ? highest : n;

    return cache[n];
}
