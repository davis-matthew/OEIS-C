/*
 * A041569  Denominators of continued fraction convergents to sqrt(302).
 * https://oeis.org/A041569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041569: Denominators of continued‑fraction convergents to sqrt(302).  
   Recurrence: a(n) = 8553246*a(n-16) - a(n-32) for n > 31.  
   Initial terms a(0)…a(31) are provided below. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static const seq_t init32[32] = {
    1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 37ULL, 82ULL, 119ULL,
    1986ULL, 2105ULL, 6196ULL, 26889ULL, 33085ULL, 59974ULL,
    93059ULL, 246092ULL, 8460187ULL, 17166466ULL, 25626653ULL,
    42793119ULL, 68419772ULL, 316472207ULL, 701364186ULL,
    1017836393ULL, 16986746474ULL, 18004582867ULL, 52995912208ULL,
    229988231699ULL, 282984143907ULL, 512972375606ULL,
    795956519513ULL, 2104885414632ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t length = 0;

    if (n >= length) {
        idx_t new_len = n + 1;
        seq_t *tmp = seq_realloc(cache, length, new_len);
        if (!tmp) return 0;           /* allocation failure – unlikely in tests */

        cache = tmp;
        for (idx_t i = length; i < new_len; ++i) {
            if (i < 32) {
                cache[i] = init32[i];
            } else {
                seq_t prod = cache[i - 16] * (seq_t)8553246;
                cache[i] = prod - cache[i - 32];
            }
        }
        length = new_len;
    }
    return cache[n];
}
