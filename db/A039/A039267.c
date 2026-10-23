/*
 * A039267  Numbers whose base-12 representation has the same number of 7's
 *          and 10's.
 * https://oeis.org/A039267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base‑12 representation
 *            contains the same number of digit 7 and digit 10 (A). This
 *            implementation builds the sequence incrementally, keeping a
 *            static cache so that calls with arbitrary n are answered in O(1)
 *            after the first expansion
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include standard library for memory allocation functions */
#include <stdlib.h>

/* Function to test if a number has equal counts of 7 and 10 in base‑12 */
static int is_good(seq_t m)
{
    /* 0 has zero 7’s and zero 10’s – it qualifies */
    if (m == (seq_t)0)
        return 1;

    int d7 = 0, d10 = 0;
    seq_t tmp = m;

    while (tmp > (seq_t)0) {
        seq_t d = tmp % (seq_t)12;          /* digit in base‑12 */
        if (d == (seq_t)7)                 /* digit 7 */
            ++d7;
        else if (d == (seq_t)10)           /* digit 10 (digit 'A') */
            ++d10;
        tmp /= (seq_t)12;                  /* move to next digit */
    }
    return d7 == d10;
}

/* Dynamic cache for the sequence values */
static seq_t *cache     = NULL;   /* array holding a(1), a(2), ... */
static idx_t cache_cap  = 0;      /* allocated size of cache       */
static idx_t cache_len  = 0;      /* number of stored terms         */
static idx_t next_m     = 0;      /* next integer to test           */

/* Ensure the cache can hold at least n terms */
static void ensure_cap(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = n;
    if (new_cap < 16)
        new_cap = 16;               /* start with a modest size */

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) {              /* allocation failed – abort */
        /* In a contest environment we could exit or set a flag,
           but for simplicity we assume enough memory. */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Offset is 1 – a(1) is first term (0). Zero or negative indices
       are undefined; we return 0 for them for safety. */
    if (n < 1)
        return (seq_t)0;

    /* Grow cache if necessary */
    if (!cache)
        ensure_cap(16);

    if (n > cache_cap)
        ensure_cap(n + 1);          /* ensure space for at least 'n' terms */

    /* Generate terms until we have enough */
    while (cache_len < n) {
        seq_t val = (seq_t)next_m;
        if (is_good(val)) {
            cache[cache_len++] = val;
        }
        ++next_m;
    }
    return cache[n - 1];
}
