/*
 * A092433  Positive numbers from the children's game "Buzz" or "Sevens":
 *          positive integers which are divisible by seven, or which contain a
 *          seven as a digit.
 * https://oeis.org/A092433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092433: a(n) is the nth positive integer divisible by 7 or containing the digit 7. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* true if x contains the decimal digit 7 */
static bool has_digit7(seq_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        if (x % 10 == 7) return true;
        x /= 10;
    }
    return false;
}

/* cache for memoised terms */
static seq_t *cache = NULL;           /* array of computed terms */
static idx_t cache_len = 0;           /* number of terms stored */
static idx_t cache_cap = 0;           /* allocated capacity */
static seq_t next_candidate = 1;      /* smallest integer not yet examined */

/* ensure cache has enough capacity for 'needed' entries */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Return the nth term of A092433 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, return 0 */

    /* compute more terms until we have at least n */
    while (cache_len < n) {
        /* find next qualifying integer */
        while (1) {
            if (next_candidate % 7 == 0 || has_digit7(next_candidate)) {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = next_candidate;
                next_candidate++;   /* advance to the next integer for future searches */
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
