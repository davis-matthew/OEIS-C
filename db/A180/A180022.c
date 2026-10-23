/*
 * A180022  Primes that can be obtained from other primes by interchanging the
 *          first and last digits. The source prime and the resulting prime
 *          are written consecutively.
 * https://oeis.org/A180022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A180022: primes that can be obtained from other primes by swapping the
 * first and last decimal digits.  Each such prime pair contributes two
 * consecutive terms: the source prime followed by the swapped prime.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Cache of generated terms */
static seq_t *cache = NULL;
static size_t  cache_len = 0, cache_cap = 0;
static seq_t  next_candidate = 2;   /* next integer to test for primality */

/* Ensure that at least n terms have been generated */
static void ensure_terms(idx_t n)
{
    if (cache_len >= n)
        return;

    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return;   /* allocation failed: terminate silently */
    }

    while (cache_len < n) {
        /* Skip non‑primes */
        if (!is_prime_big(next_candidate))
            { ++next_candidate; continue; }

        /* Ignore one‑digit primes (they would give identical swap) */
        int d = digit_len(next_candidate);
        if (d < 2) {
            ++next_candidate;
            continue;
        }

        /* Compute the swapped number */
        seq_t pow10_d_minus1 = pow10(d - 1);
        seq_t first  = next_candidate / pow10_d_minus1;
        seq_t last   = next_candidate % 10;
        seq_t middle = (next_candidate % pow10_d_minus1) / 10;
        seq_t swapped = last * pow10_d_minus1 + middle * 10 + first;

        /* If both primes, append them to the cache */
        if (is_prime_big(swapped)) {
            if (cache_len + 2 > cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    return;   /* allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
            cache[cache_len++] = swapped;
        }

        ++next_candidate;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset zero */

    ensure_terms(n);
    return cache[n - 1];
}
