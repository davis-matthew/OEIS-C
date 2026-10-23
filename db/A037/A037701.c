/*
 * A037701  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,3,0.
 * https://oeis.org/A037701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=12, a(3)=123, a(4)=1230,
   a(n) = 10*a(n-1) + d(n),  where d(n) follows the cyclic
   pattern 1,2,3,0 repeated indefinitely.  This generates
   the decimal expansion of the sequence A037701. */
#include <stdlib.h>

static const unsigned char DIGIT[4] = {1, 2, 3, 0};

static seq_t *cache = NULL;   /* 1‑based cache of computed values */
static idx_t  cached_up_to = 0; /* largest index currently cached */

/* returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for n=0 unused */

    /* grow cache if necessary */
    if (n > cached_up_to) {
        idx_t new_cap   = n;
        seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory */

        /* copy existing values if any */
        if (cache) {
            for (idx_t i = 1; i <= cached_up_to; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;

        /* compute values from (cached_up_to + 1) up to n */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)12;
            else if (i == 3) cache[i] = (seq_t)123;
            else if (i == 4) cache[i] = (seq_t)1230;
            else {
                unsigned int digit = DIGIT[(unsigned int)((i - 1) % 4)];
                cache[i] = cache[i - 1] * (seq_t)10 + (seq_t)digit;
            }
        }
        cached_up_to = n;
    }
    return cache[n];
}
