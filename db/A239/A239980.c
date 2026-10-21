/*
 * A239980  Number of n X 2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A239980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 10*a(n-3) - a(n-4) - 5*a(n-5) - 15*a(n-6) * +
 *            a(n-7) + 4*a(n-8) + 2*a(n-9) + 10*a(n-10) + 5*a(n-11) -
 *            6*a(n-13) * Initial values: a(1)=1, a(2)=3, a(3)=6, a(4)=16,
 *            a(5)=40, a(6)=84, * a(7)=208, a(8)=474, a(9)=1047, a(10)=2530,
 *            a(11)=5668, * a(12)=12907, a(13)=30446
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ *
 *  Memoised linear‑recurrence evaluator for A239980.
 *
 *  The function works for any underlying integer type `seq_t` (native
 *  unsigned integer or arbitrary‑precision integer class) and any
 *  index type `idx_t` (uint64_t).  All arithmetic uses only the
 *  operators allowed in the specification.
 *
 *  The routine keeps a static cache of computed terms; the cache is
 *  grown only when a larger `n` is requested.  Base values 1..13 are
 *  stored once and the recurrence is applied for subsequent indices.
 *  The algorithm runs in O(n) time and uses O(n) storage.
 *  ------------------------------------------------------------------ */

static seq_t *cache = NULL;       /* 1‑based cache: cache[1] = a(1) */
static idx_t  cap   = 0;          /* current cache capacity (elements) */
static idx_t  computed = 0;       /* highest index that has been filled */

/* Base values a(1)..a(13); index 0 is unused to keep the array 1‑based */
static const seq_t base[14] = {
    0ULL,          /* placeholder for 0 */
    1ULL, 3ULL, 6ULL, 16ULL, 40ULL, 84ULL,
    208ULL, 474ULL, 1047ULL, 2530ULL, 5668ULL,
    12907ULL, 30446ULL
};

/* ------------------------------------------------------------------ */
static seq_t term(idx_t n) {
    /* Ensure cache has capacity for indices up to n   */
    if (n >= cap) {
        idx_t new_cap = (n >= 64) ? (n + 1) : 64;  /* at least 64 slots */
        if (new_cap < cap) new_cap = cap * 2;      /* or double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* Out‑of‑memory fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise base terms once */
    if (computed < 13) {
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = base[i];
        computed = 13;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t t = (seq_t)0;
        t += (seq_t)2  * cache[i - 2];
        t += (seq_t)10 * cache[i - 3];
        t -=           cache[i - 4];
        t -= (seq_t)5  * cache[i - 5];
        t -= (seq_t)15 * cache[i - 6];
        t +=           cache[i - 7];
        t += (seq_t)4  * cache[i - 8];
        t += (seq_t)2  * cache[i - 9];
        t += (seq_t)10 * cache[i -10];
        t += (seq_t)5  * cache[i -11];
        t -= (seq_t)6  * cache[i -13];
        cache[i] = t;
    }

    computed = (n > computed) ? n : computed;
    return cache[n];
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n < 1 we simply
       return 0; this case is not used in the test harness. */
    if (n < 1)
        return (seq_t)0;
    return term(n);
}
