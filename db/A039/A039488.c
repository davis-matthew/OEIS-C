/*
 * A039488  Numbers whose base-11 representation has the same nonzero number
 *          of 7's and 9's.
 * https://oeis.org/A039488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the OEIS sequence A039488.
 *  Sequence: Numbers whose base‑11 representation has the same non‑zero
 *  number of 7’s and 9’s.
 *  The function returns a(1) = 86, a(2) = 106, … as described in the
 *  problem statement.
 *
 *  The implementation keeps a cache of discovered values.  If seq_t is a
 *  built‑in type (C compilation) it uses malloc/realloc for the cache.
 *  If seq_t is an arbitrary‑precision class (> C++03) it uses
 *  std::vector, which correctly constructs and destroys the objects.
 *
 *  The logic for generating the next valid integer is the same in both
 *  flavourings: repeatedly test successive integers until a number
 *  having equal non‑zero counts of 7 and 9 in base‑11 is found.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- helper to check a number ---------- */
static inline bool is_valid(seq_t x)
{
    unsigned cnt7 = 0, cnt9 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % (seq_t)11);
        if (d == 7)      ++cnt7;
        else if (d == 9) ++cnt9;
        x /= (seq_t)11;
    }
    return (cnt7 == cnt9) && (cnt7 != 0);
}

/* ---------- cache handling for C ---------- */
#ifndef __cplusplus
static seq_t *cache   = NULL;   /* array of cached terms */
static size_t cache_cap = 0;    /* capacity of cache array   */
static size_t cache_cnt = 0;    /* number of cached terms    */
#else
/* ---------- cache handling for C++ ---------- */
static std::vector<seq_t> cache_vec;
#endif

static seq_t next_candidate = 1;   /* next integer to test */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

#ifdef __cplusplus
    /* grow the vector until it contains the nth term */
    while (cache_vec.size() < (size_t)n) {
        while (!is_valid(next_candidate))
            ++next_candidate;
        cache_vec.push_back(next_candidate);
        ++next_candidate;
    }
    return cache_vec[(size_t)n - 1];
#else
    /* ensure the cache array can hold n terms */
    while (cache_cnt < (size_t)n) {
        /* find the next valid number */
        while (!is_valid(next_candidate))
            ++next_candidate;

        /* allocate more space if needed */
        if (cache_cnt + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_cnt++] = next_candidate;
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
#endif
}
