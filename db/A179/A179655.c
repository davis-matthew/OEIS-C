/*
 * A179655  Digital root of n-th abundant number.
 * https://oeis.org/A179655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digital_root( n-th abundant number ) (n≥1)
 * Range:     u8 n <= 58, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache of abundant numbers */
static seq_t *abundant = NULL;
static size_t abundant_count   = 0;   /* number of entries in cache */
static size_t abundant_capacity = 0;  /* allocated size of the cache */
static idx_t  max_checked       = 0;  /* last integer examined */

/* enlarge cache capacity */
static void grow_cache(size_t min_capacity)
{
    if (abundant_capacity >= min_capacity) return;
    size_t new_cap = abundant_capacity ? abundant_capacity * 2 : 128;
    if (new_cap < min_capacity) new_cap = min_capacity;
    seq_t *new_arr = (seq_t *)realloc(abundant, new_cap * sizeof(seq_t));
    abundant = new_arr;
    abundant_capacity = new_cap;
}

/* extend the cache until it contains at least 'target' abundant numbers */
static void ensure_abundant(idx_t target)
{
    while (abundant_count < target) {
        for (idx_t m = max_checked + 1; abundant_count < target; ++m) {
            max_checked = m;
            if (m <= 1) continue;          /* 1 is not abundant */
            uint64_t sum = 1;              /* 1 is always a proper divisor */
            for (idx_t d = 2; d <= m / d; ++d) {
                if (m % d == 0) {
                    sum += d;
                    if (d != m / d) sum += m / d;
                }
            }
            if (sum > m) {
                grow_cache(abundant_count + 1);
                abundant[abundant_count++] = (seq_t)m;
            }
        }
    }
}

/* public function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n<1 */
    ensure_abundant(n);
    seq_t m = abundant[n - 1];
    /* digital root: 1 + ((m – 1) mod 9)  – works for all positive m */
    return (seq_t)(1 + ((m - 1) % (seq_t)9));
}
