/*
 * A206872  Number of 3 X n 0..1 arrays avoiding 0 0 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 7*a(n-2) + 27*a(n-3) - 13*a(n-4) + a(n-5)
 *               - 31*a(n-6) + 5*a(n-7) - 2*a(n-8) + 4*a(n-9)
 *       for n > 9.
 *
 * Initial values (1 ≤ n ≤ 9):
 *   a(1)=7, a(2)=49, a(3)=211, a(4)=1153, a(5)=6139,
 *   a(6)=31529, a(7)=165783, a(8)=867545, a(9)=4529439.
 *
 * The sequence is computed by the above linear recurrence.  The function
 * keeps a static memoised table so that calls may arrive in any order
 * and are answered in O(n) time per call (after the first call).  The
 * table is resized with `realloc`, so for an arbitrary‑precision build
 * the pointer is cast appropriately.  The implementation uses only
 * integer arithmetic and should compile as both C11 and C++17.
 */

#include <stdlib.h>
#include <stddef.h>

/* static memoisation table, 0‑based indexing; cache[i] is a(i+1)    */
static seq_t *cache = NULL;
/* number of entries already stored (1 = a(1), 2 = a(2), …)          */
static idx_t cached_len = 0;
/* current capacity of the table (in entries)                        */
static size_t cached_cap = 0;

/* enlarge the table so that it can hold at least `n` entries      */
static void ensure_capacity(idx_t n)
{
    if (cached_cap < (size_t)n) {
        size_t new_cap = cached_cap > 0 ? cached_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        /* cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cached_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, but return 0 to avoid crash */

    /* initialise the first nine terms on first use */
    if (cached_cap == 0) {
        cached_cap = 16;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache)
            abort();

        enum { NINIT = 9 };
        const seq_t init[NINIT] = {
            /* a(1) … a(9) */
            7ULL,          49ULL,          211ULL,
            1153ULL,       6139ULL,       31529ULL,
            165783ULL,     867545ULL,    4529439ULL
        };
        for (size_t i = 0; i < NINIT; ++i)
            cache[i] = init[i];
        cached_len = NINIT;
    }

    if (n <= cached_len)
        return cache[(size_t)(n - 1)];

    /* extend the cache up to n using the recurrence */
    ensure_capacity(n);
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);    /* 0‑based index for cache */

        /* fetch the last nine values */
        seq_t a1 = cache[idx - 1];
        seq_t a2 = cache[idx - 2];
        seq_t a3 = cache[idx - 3];
        seq_t a4 = cache[idx - 4];
        seq_t a5 = cache[idx - 5];
        seq_t a6 = cache[idx - 6];
        seq_t a7 = cache[idx - 7];
        seq_t a8 = cache[idx - 8];
        seq_t a9 = cache[idx - 9];

        /* compute the recurrence */
        seq_t pos = 3 * a1;
        pos += 7 * a2;
        pos += 27 * a3;
        pos += a5;
        pos += 5 * a7;
        pos += 4 * a9;

        seq_t neg = 13 * a4;
        neg += 31 * a6;
        neg += 2 * a8;

        cache[idx] = pos - neg;   /* a(i) */
    }

    cached_len = n;
    return cache[(size_t)(n - 1)];
}
