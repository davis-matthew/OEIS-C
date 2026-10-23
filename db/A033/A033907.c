/*
 * A033907  Sort then Add, a(1)=32.
 * https://oeis.org/A033907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 32; for n>1, a(n) = a(n‑1) + sorted_digits(a(n‑1)), where
 *            sorted_digits(x) is the integer obtained by sorting the decimal
 *            digits of x in ascending order
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* helper: return the digits of x sorted in increasing order */
static seq_t sort_digits(seq_t x) {
    unsigned char cnt[10] = {0};
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        ++cnt[d];
        tmp /= (seq_t)10;
    }
    seq_t res = (seq_t)0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned char c = 0; c < cnt[d]; ++c) {
            res = res * (seq_t)10 + (seq_t)d;
        }
    }
    return res;
}

/* memoised recurrence */
seq_t a(idx_t n) {
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for 0 or negative n */

    static seq_t *cache = NULL;   /* holds a(1) .. a(used) */
    static size_t used = 0;       /* number of terms stored */
    static size_t cap = 0;        /* allocated capacity */

    size_t sz = (size_t)n;        /* n is expected to fit in size_t for loops */

    if (sz > used) {
        /* grow capacity if needed */
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap < sz) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;

        /* initialise the first term if necessary */
        if (used == 0) {
            cache[0] = (seq_t)32;
            used = 1;
        }

        /* compute remaining terms up to n */
        for (size_t i = used; i < sz; ++i) {
            seq_t prev = cache[i - 1];
            seq_t sorted = sort_digits(prev);
            cache[i] = prev + sorted;
        }
        used = sz;
    }
    return cache[sz - 1];
}
