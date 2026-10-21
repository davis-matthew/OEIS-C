/*
 * A221644  Let abcd... be the decimal expansion of k. Sequence lists numbers
 *          k such that 1/a + 2/b + 3/c + 4/d + ... is an integer.
 * https://oeis.org/A221644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 992, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A221644.
   The function enumerates positive integers in increasing order,
   skips those containing the digit 0, and keeps those for which
   S = Σ i/d_i (digits from most significant side) is an integer.
   It caches all found terms so subsequent calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;   /* largest integer already examined */

/* Append a term to the cache, growing the array if necessary */
static void append_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the nth term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but never called */

    /* Extend the cache until we have at least n terms */
    while (cache_size < n) {
        ++last_checked;            /* examine the next integer */

        uint64_t t = last_checked;
        if (t == 0) continue;      /* skip 0, though it doesn't happen */

        /* Extract digits into a buffer, counting from least significant */
        uint8_t digits[20];
        int len = 0;
        bool has_zero = false;
        while (t > 0) {
            uint8_t d = t % 10;
            if (d == 0) { has_zero = true; break; }
            digits[len++] = d;
            t /= 10;
        }
        if (has_zero) continue;    /* skip numbers with a zero digit */

        /* Compute product of digits */
        seq_t prod = 1;
        for (int i = 0; i < len; ++i) prod *= (seq_t)digits[i];

        /* Compute the numerator of the sum S = Σ i/d_i (denominator = prod) */
        seq_t sum = 0;
        for (int pos = 1; pos <= len; ++pos) {
            uint8_t d = digits[len - pos];   /* leftmost digit has pos=1 */
            seq_t term = (seq_t)pos * (prod / (seq_t)d);
            sum += term;
        }

        if (sum % prod == 0) {            /* S is an integer */
            append_cache((seq_t)last_checked);
        }
    }

    return cache[n - 1];
}
