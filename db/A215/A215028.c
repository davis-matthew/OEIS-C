/*
 * A215028  a(1) = 1; for n >= 1, a(n+1) = (concatenation of n+1 and n) -
 *          a(n).
 * https://oeis.org/A215028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n >= 1, a(n+1) = concat(n+1, n) - a(n)
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digits, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;   /* number of terms stored, cache[0] = a(1) */

    if (len == 0) {
        /* Initial capacity: small but sufficient for moderate n; grow as needed */
        cap = 128;
        cache = seq_realloc(NULL, 0, cap);
        cache[0] = (seq_t)1;   /* a(1) */
        len = 1;
    }

    /* Grow cache if needed */
    if (n > cap) {
        size_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to a(n) */
    while (len < n) {
        /* We are currently at term a(len). We need a(len+1) */
        idx_t curr = (idx_t)len;          /* current n value (>=1) */
        seq_t n_seq = (seq_t)curr;
        seq_t prev = cache[len - 1];      /* a(curr) */
        /* concat(curr+1, curr) = (curr+1)*10^digits(curr) + curr */
        seq_t pow = pow10((idx_t)digits(curr));
        seq_t concat = (n_seq + 1) * pow + n_seq;
        cache[len] = concat - prev;       /* a(curr+1) */
        ++len;                            /* now len == curr+1 */
    }

    return cache[n - 1];
}
