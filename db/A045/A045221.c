/*
 * A045221  Numbers whose base-5 representation contains exactly one 1 and no
 *          2's.
 * https://oeis.org/A045221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains exactly one 1 and no 2's
   (OEIS A045221). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Helper: test whether a value satisfies the definition ---------------- */

/* Return true if val has exactly one digit '1' in base‑5 and no digit '2'. */
static bool passes(seq_t val)
{
    bool one_found = false;
    while (val > 0) {
        unsigned digit = (unsigned)(val % 5);   /* digit is 0..4 */
        if (digit == 2)
            return false;                      /* 2 not allowed */
        if (digit == 1) {
            if (one_found)
                return false;                  /* more than one 1 */
            one_found = true;
        }
        val /= 5;
    }
    return one_found;                           /* must have exactly one 1 */
}

/* --- Cache infrastructure ------------------------------------------------ */

/* Cached sequence terms */
static seq_t *cache     = NULL;   /* dynamically allocated array              */
static size_t cache_len = 0;      /* number of terms stored in cache          */
static size_t cache_cap = 0;      /* allocated capacity of cache              */
static seq_t next_candidate = 1;  /* next integer to test                     */
static idx_t found_cnt = 0;       /* how many terms have been found so far    */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* If allocation fails, abort; the test harness will catch it. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate sequence terms until we have found at least 'n' terms. */
static void generate_up_to(idx_t n)
{
    while (found_cnt < n) {
        if (passes(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
            ++found_cnt;
        }
        ++next_candidate;   /* move to next integer */
    }
}

/* --- Public API ----------------------------------------------------------- */

/* Return the n‑th term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1 */
    generate_up_to(n);
    return cache[n - 1];
}
