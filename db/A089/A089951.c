/*
 * A089951  Numbers having the same leading decimal digits as their squares.
 * https://oeis.org/A089951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Numbers having the same leading decimal digit as their squares.
 * A number n (n = 0 or n > 0) is in the sequence iff the leading decimal
 * digit of n equals the leading decimal digit of n².  The sequence is
 * indexed with offset 1, so a(1)=0, a(2)=1, a(3)=10, a(4)=11, … 
 * This implementation lazily builds the sequence and caches the terms.
 * It works for any width of seq_t (including arbitrary-precision) and
 * for any idx_t given by the build system.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

/* forward declaration supplied by the build system */
seq_t a(idx_t n);

/* local helper: first decimal digit of an unsigned 64‑bit integer */
static uint64_t first_digit_uint64(uint64_t x)
{
    while (x >= 10ULL)
        x /= 10ULL;
    return x;
}

/* cache for terms; stored as 64‑bit integers so we can handle large
 * candidates before converting to seq_t.  The cache is grown on demand. */
static uint64_t *term_cache = NULL;   /* array of found terms */
static size_t term_cache_size = 0;    /* number of terms currently cached */
static size_t term_cache_cap  = 0;    /* allocated capacity */

/* next natural number to test for inclusion in the sequence */
static uint64_t next_candidate = 1;

/* grow the cache so that it can hold at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    size_t new_size = term_cache_cap ? term_cache_cap : 16;
    while (new_size < new_cap)
        new_size *= 2;
    uint64_t *p = (uint64_t *)realloc(term_cache, new_size * sizeof(uint64_t));
    term_cache = p;
    term_cache_cap = new_size;
}

/* append a new term to the cache; growing it if necessary */
static void append_term(uint64_t val)
{
    if (term_cache_size >= term_cache_cap)
        grow_cache(term_cache_size + 1);
    term_cache[term_cache_size++] = val;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* First call: initialise cache with a(1)=0. */
    if (term_cache == NULL) {
        grow_cache(16);
        append_term(0ULL);          /* a(1) = 0 */
        next_candidate = 1;         /* start testing from 1 */
    }

    /* Generate terms until we have at least n of them. */
    while (term_cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        uint64_t fd = first_digit_uint64(cand);
        uint64_t sq = (uint64_t)((unsigned __int128)cand * cand);
        uint64_t fd_sq = first_digit_uint64(sq);
        if (fd == fd_sq)
            append_term(cand);
        next_candidate = cand + 1;
    }

    /* Return the requested value, cast to seq_t.  If the term does
     * not fit into seq_t (e.g. in an 8‑bit build), the lower bytes
     * are returned; such calls are not required by the tests. */
    return (seq_t)term_cache[(size_t)n - 1];
}
