/*
 * A036433  Number of divisors is a digit in the base 10 representation of n.
 * https://oeis.org/A036433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036433: numbers whose divisor count is a digit of the number.
 * The nth term is the nth such integer. Computed by searching successive
 * integers and testing the property. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper: check if a decimal number contains a particular digit ---------- */
static bool contains_digit(uint64_t number, uint8_t digit)
{
    if (digit > 9) return false;              /* irrelevant digit                     */
    if (number == 0) return digit == 0;       /* safety: not needed for this seq      */

    while (number > 0) {
        if ((number % 10) == digit) return true;
        number /= 10;
    }
    return false;
}

/* ---------- helper: compute number of positive divisors of n (uint64_t) ---------- */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;                     /* d(1)=1, d(0) not used               */
    uint64_t count = 0;
    uint64_t r = 0;
    /* integer sqrt via simple loop (sufficient for the expected size) */
    for (r = 1; (uint64_t)(r * r) <= n; ++r) { /* r grows until r^2 > n            */
        ; /* empty: just to find r */
    }
    --r; /* r is now floor(sqrt(n)) */

    for (uint64_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            count += (i * i == n) ? 1 : 2;
        }
    }
    return count;
}

/* ---------- helper: does n satisfy the property? ---------- */
static bool meets_property(uint64_t n)
{
    uint64_t d = divisor_count_u64(n);
    if (d < 1 || d > 9) return false;        /* must be a single decimal digit       */
    return contains_digit(n, (uint8_t)d);
}

/* ---------- cache for the sequence values ---------- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static uint64_t last_checked = 0;

/* grow the cache to at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);            /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- main sequence access function ---------- */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;                     /* undefined for offset 0, harmless    */

    /* generate terms up to the requested index */
    while (cache_size < (size_t)n) {
        ++last_checked;
        if (meets_property(last_checked)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
