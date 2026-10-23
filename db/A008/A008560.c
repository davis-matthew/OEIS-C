/*
 * A008560  a(1) = 2; to get a(n), n >= 2, convert a(n-1) from base 3 to base
 *          2.
 * https://oeis.org/A008560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; for n ≥ 2, a(n) is the value obtained by interpreting
 *            a(n‑1) as a base‑3 number and then writing that number in
 *            base‑2. All terms are represented as decimal integers consisting
 *            only of digits 0 and 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] holds a(1) */
static idx_t cache_size = 0;  /* allocated elements */

/* Ensure the cache can hold at least `needed` terms. */
static void ensure_cache(idx_t needed)
{
    if (cache_size >= needed)
        return;
    idx_t new_size = cache_size ? cache_size : 1;
    while (new_size < needed)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;
}

/* Return the nth term (1‑based).  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safe fallback */

    static idx_t computed = 0;      /* number of terms in the cache */

    if (computed < n) {
        ensure_cache(n);            /* enlarge cache if necessary */

        for (idx_t i = computed; i < n; ++i) {
            seq_t value;
            if (i == 0) {
                /* a(1) = 2 */
                value = 2;
            } else {
                /* Step 1: read a(i) in decimal, interpret as base-3
                   and obtain its decimal value. */
                seq_t prev = cache[i - 1];
                seq_t base3_val = 0;
                seq_t mul = 1;
                seq_t tmp = prev;
                while (tmp > 0) {
                    seq_t digit = tmp % 10;   /* decimal digit 0/1/2 */
                    base3_val += digit * mul;
                    mul *= 3;
                    tmp /= 10;
                }

                /* Step 2: convert that value to binary digits,
                   but store the binary digits as a decimal integer. */
                seq_t cur = 0;
                seq_t mult = 1;
                while (base3_val > 0) {
                    seq_t bit = base3_val % 2;
                    cur += bit * mult;
                    mult *= 10;
                    base3_val /= 2;
                }
                if (cur == 0) cur = 0; /* handles the (unlikely) zero */
                value = cur;
            }
            cache[i] = value;
        }
        computed = n;
    }
    return cache[n - 1];
}
