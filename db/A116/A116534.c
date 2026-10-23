/*
 * A116534  Start with 1 and 2; if number is even concatenate with last number
 *          and divide by 2, if it is odd add 1 and divide by 2.
 * https://oeis.org/A116534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=2; for n>=2, if a(n-1) odd: a(n)=(a(n-1)+1)/2; else
 *            a(n)=concat(a(n-2),a(n-1))/2. Equivalent to
 *            a(n)=ceil(a(n-1)/2)+(1+(-1)^a(n-1))*10^floor(log10(a(n-1))+1)*a(n-2)/4
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t last_n = (idx_t)(-1);

    if (n < 2) {
        return (idx_t)(n == 0 ? 1 : 2);
    }

    if (cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) {
                abort();   /* allocation failed */
            }
        }
        cache = new_cache;
        cap = new_cap;
    }

    if (last_n == (idx_t)(-1)) {
        cache[0] = 1;
        if (cap > 1) cache[1] = 2;
        last_n = 1;
    }

    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;
        if ((prev & 1) == 0) {          /* even */
            seq_t prevprev = cache[i - 2];
            idx_t d = (idx_t)digit_len(prev);
            seq_t pow10d = pow10(d);
            seq_t concat = prevprev * pow10d + prev;
            next = concat / 2;
        } else {                       /* odd */
            next = (prev + 1) / 2;
        }
        cache[i] = next;
    }
    last_n = n;
    return cache[n];
}
