/*
 * A154528  Composites k such that (smallest digit of k) + (multiplicity of
 *          smallest digit of k) is an even composite.
 * https://oeis.org/A154528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of composites k such that (smallest digit of k) +
   multiplicity of the smallest digit is an even composite. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/digit_frequencies.h"

/* static cache of found terms */
static seq_t *seq_cache = NULL;        /* array of seq_t values */
static size_t seq_cache_len = 0;       /* number of terms cached */
static size_t seq_cache_cap = 0;       /* allocated capacity */
static seq_t seq_last_num = 4;         /* next candidate to test */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (seq_cache_cap >= needed) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *p = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = p;
    seq_cache_cap = new_cap;
}

/* true if the smallest digit of k plus its multiplicity is an
   even composite number */
static bool satisfies_property(seq_t k)
{
    unsigned int freq[10] = {0};
    digit_frequencies(k, freq);

    int d = -1;
    unsigned int m = 0;
    for (int i = 0; i <= 9; ++i) {
        if (freq[i]) { d = i; m = freq[i]; break; }
    }
    /* d is the smallest non‑zero digit, m its count */
    int sum = d + (int)m;
    return (sum % 2 == 0 && sum >= 4 && !is_prime_small((seq_t)sum));
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined below offset 1 */

    /* generate terms until we reach the requested index */
    while (seq_cache_len < (size_t)n) {
        /* find the next composite with the required property */
        for (;;) {
            if (!is_prime_small(seq_last_num)) {
                if (satisfies_property(seq_last_num)) {
                    ensure_cache_cap(seq_cache_len + 1);
                    seq_cache[seq_cache_len++] = seq_last_num;
                    seq_last_num = seq_last_num + (seq_t)1; /* next candidate */
                    break;
                }
            }
            seq_last_num = seq_last_num + (seq_t)1;
        }
    }
    return seq_cache[n - 1];
}
