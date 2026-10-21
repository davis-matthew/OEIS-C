/*
 * A060815  Powers of 2 with each digit a power of 2 (only digits 0,1,2,4,8
 *          are used).
 * https://oeis.org/A060815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   allowed_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A060815 - Enumerate powers of 2 and keep those whose decimal
 * digits belong only to {0,1,2,4,8}. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/allowed_digits.h"

/* cache of found terms */
static seq_t *term_cache = NULL;
static size_t term_len   = 0;   /* number of terms stored so far   */
static size_t term_cap   = 0;   /* allocated capacity of cache      */

/* next power of two that will be considered */
static idx_t  next_exp   = 0;   /* exponent of next_power          */
static seq_t  next_power = 0;   /* value of the next power of two   */

/* grow the cache to hold at least `need` elements */
static void ensure_cap(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = term_cap ? term_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure will be ignored on tests */
    term_cache = new_cache;
    term_cap   = new_cap;
}

/* return the n-th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 – safe guard   */
    while (term_len < n) {
        if (next_exp == 0) {         /* first power: 2^1 = 2              */
            next_power = 2;
            next_exp   = 1;
        } else {
            next_power <<= 1;        /* multiply by 2: next power of two  */
            ++next_exp;
        }
        if (allowed_digits(next_power)) {
            ensure_cap(term_len + 1);
            term_cache[term_len++] = next_power;
        }
    }
    return term_cache[n - 1];
}
