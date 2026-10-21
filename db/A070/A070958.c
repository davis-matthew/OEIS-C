/*
 * A070958  Numbers n such that the largest digit of n is the same as the
 *          largest digit of (sum of digits of n).
 * https://oeis.org/A070958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th nonnegative integer for which the largest
 *            decimal digit of the integer equals the largest decimal digit of
 *            the sum of its decimal digits. a(1) = 0, a(2) = 1, …
 * Range:     u8 n <= 22, u16 n <= 6566, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len  = 0;            /* number of cached terms */
static size_t cache_cap  = 0;            /* allocated cache capacity */
static idx_t  last_checked = 0;          /* next integer to evaluate */

/* largest digit of a non‑negative decimal integer (0 .. 9) */
static seq_t largest_digit(seq_t x)
{
    seq_t m = 0;
    while (x) {
        seq_t d = x % 10;
        if (d > m) m = d;
        x /= 10;
    }
    return m;
}

/* sum of decimal digits of a non‑negative decimal integer */
static seq_t digit_sum(seq_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* enlarge the cache when needed; uses malloc/free for portability to
   arbitrary‑precision seq_t types (which may have non‑trivial copy
   constructors). */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term; terms are generated lazily.
   The function is thread‑safe only in a single‑threaded context
   (which is typical for the evaluation harness). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined offset – harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)last_checked;
        seq_t lnd  = largest_digit(cand);
        seq_t lds  = largest_digit(digit_sum(cand));

        if (lnd == lds) {
            if (cache_len == cache_cap) grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        last_checked++;
    }

    return cache[(size_t)n - 1];
}
