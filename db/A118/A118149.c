/*
 * A118149  Start with 1 and repeatedly reverse the digits and add 52 to get
 *          the next term.
 * https://oeis.org/A118149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1)=1; for n>1, a(n)=reverse(a(n-1))+52
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reverse the decimal digits of a value of type seq_t */
static seq_t reverse_digits(seq_t val) {
    seq_t rev = 0;
    while (val > 0) {
        rev = rev * 10 + val % 10;
        val /= 10;
    }
    return rev;
}

/* Compute a(n) with memoization */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1, undefined for 0 but harmless */

    static seq_t *cache = NULL;  /* array of computed terms */
    static idx_t len = 0;        /* number of terms stored */
    static size_t cap = 0;       /* allocated capacity */

    /* extend cache up to index n-1 (since a(1) stored at 0) */
    while (len < n) {
        /* ensure capacity */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (!newarr) return 0;  /* allocation failure: undefined behavior */
            if (cache) {
                for (idx_t i = 0; i < len; ++i) newarr[i] = cache[i];
                free(cache);
            }
            cache = newarr;
            cap = newcap;
        }

        seq_t next;
        if (len == 0) {
            next = (seq_t)1;          /* a(1) */
        } else {
            seq_t rev = reverse_digits(cache[len - 1]);
            next = rev + (seq_t)52;
        }
        cache[len++] = next;
    }

    return cache[n - 1];
}
