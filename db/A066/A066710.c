/*
 * A066710  RATS: Reverse Add Then Sort the digits applied to previous term,
 *          starting with 3.
 * https://oeis.org/A066710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Reverse Add Then Sort the digits applied to previous
 *            term, starting with 3. Recurrence: a(1)=3, a(n+1)=A036839(a(n))
 *            for n>=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   digit_frequencies, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes the values up to the required index. */

#include <stddef.h>
#include "../utils/reverse.h"          /* provides seq_t reverse(seq_t v) */
#include "../utils/digit_frequencies.h"/* provides void digit_frequencies(seq_t x, unsigned int freq[10]) */

/* Maximum cache size – sufficient for the expected n (≤ 10000). */
#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* Compute the next term from the current term. */
static seq_t next_term(seq_t k)
{
    seq_t rev = reverse(k);
    seq_t sum = k + rev;

    unsigned freq[10] = {0};
    digit_frequencies(sum, freq);

    seq_t result = 0;
    for (unsigned d = 0; d < 10; ++d) {
        for (unsigned i = 0; i < freq[d]; ++i) {
            result = result * 10 + d;
        }
    }
    return result;
}

/* Public interface as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, behaviour for n=0 irrelevant */
    if (n > MAX_CACHE) return 0; /* out of bounds – caller responsibility */

    /* Initialize cache if needed. */
    if (cache_len == 0) {
        cache[0] = 3;
        cache_len = 1;
    }

    /* Extend cache up to index n. */
    while (cache_len < n) {
        cache[cache_len] = next_term(cache[cache_len - 1]);
        ++cache_len;
    }

    return cache[n - 1];
}
