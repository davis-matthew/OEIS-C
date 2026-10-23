/*
 * A044877  Numbers having, in base 6, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑6 representation has equal sum of even and odd run lengths */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper that checks the run‑length condition for a single number */
static bool good(seq_t val)
{
    seq_t temp = val;
    bool seen = false;
    unsigned previous_digit = 0;
    unsigned run_len = 0;
    idx_t even_sum = 0;
    idx_t odd_sum = 0;

    while (temp > 0) {
        unsigned digit = (unsigned)(temp % (seq_t)6);
        if (!seen) {
            previous_digit = digit;
            run_len = 1;
            seen = true;
        } else if (digit == previous_digit) {
            run_len++;
        } else {
            if ((run_len & 1u) == 0)
                even_sum += run_len;
            else
                odd_sum += run_len;
            previous_digit = digit;
            run_len = 1;
        }
        temp = temp / (seq_t)6;
    }

    if (seen) {
        if ((run_len & 1u) == 0)
            even_sum += run_len;
        else
            odd_sum += run_len;
    }

    return even_sum == odd_sum;
}

seq_t a(idx_t n)
{
    /* static cache to allow repeated out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* largest value already considered */

    if (n == 0) return 0;          /* safety for calls with bad indices */

    /* already computed? */
    if (n <= cache_size)
        return cache[n - 1];

    /* first call: create initial cache */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    seq_t candidate = last_checked;
    while (cache_size < n) {
        candidate = candidate + 1;          /* test next integer */
        if (good(candidate)) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = candidate;
            last_checked = candidate;
        }
    }

    return cache[n - 1];
}
