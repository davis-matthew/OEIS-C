/*
 * A121808  Say what you see in previous term, reporting each digit
 *          encountered followed by frequency.
 * https://oeis.org/A121808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; for n>1, a(n) is formed by * reading the decimal
 *            representation of a(n‑1), * counting the occurrences of each
 *            digit 0‑9, * and writing the digit followed by its count * (only
 *            for digits that appear), in increasing * digit order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_frequencies, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes required by the helper utilities. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"   /* void digit_frequencies(seq_t x, unsigned int freq[10]); */
#include "../utils/seq_one.h"             /* seq_t seq_one(void); */

/*  Compute one step of the sequence using only arithmetic
 *  on seq_t values.  The function assumes that freq[d]
 *  contains the number of times digit d appears in the
 *  decimal representation of prev. */
static seq_t compute_next(seq_t prev)
{
    unsigned int freq[10];
    digit_frequencies(prev, freq);

    seq_t result = (seq_t)0;
    for (int d = 0; d < 10; ++d) {
        if (freq[d] == 0)
            continue;

        /* Append the digit itself. */
        result = result * (seq_t)10 + (seq_t)d;

        /* Append the decimal digits of the count. */
        unsigned int cnt = freq[d];
        /* Store the digits of cnt in reverse order. */
        unsigned int tmp = cnt;
        unsigned int digits[10];
        int len = 0;
        do {
            digits[len++] = tmp % 10;
            tmp /= 10;
        } while (tmp);

        for (int i = len - 1; i >= 0; --i)
            result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}

/*  Cache the computed sequence values so that requests
 *  for the same or smaller indices are answered in O(1). */
static seq_t *cache = NULL;   /* array of a(1), a(2), ... */
static idx_t cache_size = 0;  /* number of entries currently stored */

/*  Main sequence function requested by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, return 0 */

    /* Allocate space for the required number of terms if necessary. */
    if (cache_size < n) {
        size_t new_size = n;
        seq_t *tmp = NULL;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        if (!tmp)
            return (seq_t)0;         /* allocation failure – unlikely */

        cache = tmp;

        /* Compute terms from the first missing index up to n. */
        if (cache_size == 0) {
            cache[0] = seq_one();    /* a(1) = 1 */
            cache_size = 1;
        }

        for (idx_t i = cache_size; i < n; ++i)
            cache[i] = compute_next(cache[i - 1]);

        cache_size = n;
    }

    return cache[n - 1];
}
