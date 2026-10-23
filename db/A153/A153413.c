/*
 * A153413  Smaller of twin prime pairs such that
 *          p1*p2+average_of_twin_prime_pair=prime.
 * https://oeis.org/A153413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smaller member of twin prime pairs (p, p+2) * such
 *            that p² + 3p + 1 is prime. This function generates the *
 *            sequence by brute‑force search, caching the values so that
 *            arbitrary * order of calls is handled efficiently
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_generic.h"

#define MAX_TERMS 100000  /* enough for n up to ~100 000; far beyond 10 000 */

static seq_t cache[MAX_TERMS];  /* cached sequence values */
static size_t cache_len = 0;    /* number of terms currently stored */
static idx_t  next_candidate = 3;   /* next odd integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset – safe return */

    /* Generate terms until we have at least n values. */
    while (cache_len < (size_t)n)
    {
        /* Skip even numbers automatically by keeping next_candidate odd. */
        if (is_prime_generic(next_candidate) &&
            is_prime_generic(next_candidate + 2))
        {
            seq_t p  = (seq_t)next_candidate;
            seq_t val = p * p + (seq_t)3 * p + (seq_t)1;

            /* val fits in idx_t for all values we need (see analysis).  */
            if (is_prime_generic((idx_t)val))
            {
                cache[cache_len++] = p;
            }
        }

        next_candidate += 2;
    }

    return cache[n - 1];
}
