/*
 * A143373  Expansion of x/(1 - x - 2*x^3 - 2*x^5 - x^7).
 * https://oeis.org/A143373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 76, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A143373: expansion of x/(1 - x - 2*x^3 - 2*x^5 - x^7)
 *
 * Recurrence:
 *   a(1) = 1
 *   a(2) = 1
 *   a(3) = 1
 *   a(n) = a(n‑1) + 2*a(n‑3) + 2*a(n‑5) + a(n‑7)      for n ≥ 4
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *buf = NULL;
    static size_t cap = 0;
    static size_t len = 0;              /* highest index computed (0 means none) */

    /* index is 1‑based; buf[0] is unused, set to 0 for convenience */
    size_t target = (size_t)n;          /* n is never > 10000 in tests */

    /* allocate or grow buffer if needed */
    if (target > len) {
        size_t need = target + 1;       /* space for indices 0 .. target */
        if (cap < need) {
            size_t newcap = cap ? cap * 2 : 8;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp) abort();         /* allocation failed */
            /* initialise new entries to 0 (they are not used until computed) */
            for (size_t i = cap; i < newcap; ++i) tmp[i] = (seq_t)0;
            buf = tmp;
            cap = newcap;
        }

        /* compute values from len+1 up to target */
        for (size_t i = len + 1; i <= target; ++i) {
            seq_t val = 0;
            if (i >= 1) val += buf[i - 1];
            if (i >= 3) val += (seq_t)2 * buf[i - 3];
            if (i >= 5) val += (seq_t)2 * buf[i - 5];
            if (i >= 7) val += buf[i - 7];
            if (i == 1) val += (seq_t)1;   /* a(1) = 1 */
            buf[i] = val;
        }
        len = target;
    }

    return buf[n];
}
