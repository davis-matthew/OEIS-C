/*
 * A103632  Expansion of (1 - x + x^2)/(1 - x - x^4).
 * https://oeis.org/A103632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-4), a(0)=1, a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoized reference implementation. */
    static seq_t *buf = NULL;      /* store computed values */
    static idx_t buf_len = 0;      /* allocated length */
    static idx_t computed = 0;     /* number of entries already computed */

    /* Directly return known initial values for n < 4. */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)1;
        }
    }

    idx_t need = n + 1;   /* number of elements required */

    /* Grow buffer if necessary. */
    if (need > buf_len) {
        size_t new_len = need;
        if (buf_len < 4)               /* ensure space for the four base values */
            new_len = need > 4 ? need : 4;

        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure – unlikely for the test range */

        buf = tmp;
        buf_len = new_len;

        /* Initialise the first four terms once. */
        if (computed < 4) {
            buf[0] = (seq_t)1;
            buf[1] = (seq_t)0;
            buf[2] = (seq_t)1;
            buf[3] = (seq_t)1;
            computed = 4;
        }
    }

    /* Compute any new terms up to n using the recurrence. */
    for (idx_t i = computed; i <= n; ++i) {
        buf[i] = buf[i - 1] + buf[i - 4];
    }
    computed = n + 1;

    return buf[n];
}
