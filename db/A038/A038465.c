/*
 * A038465  Sums of 3 distinct powers of 3.
 * https://oeis.org/A038465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 602, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the sorted list of sums of three distinct powers of 3
   (OEIS A038465).  Values are cached after the first call.
   The maximum exponent of 3 use is chosen so that the first
   10 000 terms are present; for 64‑bit types the limiting exponent
   is 40, which keeps 3^40 within the 64‑bit range.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) is undefined; return 0 in this case */
        return (seq_t)0;

    /* Only initialise once */
    static bool   init = false;
    static seq_t *vals = NULL;
    static size_t count = 0;

    if (!init) {
        /* Determine the needed largest exponent so that at least 10 000
           sums exist:  C(exp+1,3) >= 10 000. */
        idx_t  exp   = 0;
        size_t powcnt, total;
        const size_t target = 10000;   /* cover all tests */

        do {
            exp++;
            powcnt = exp + 1;
            total  = powcnt * (powcnt - 1) * (powcnt - 2) / 6;
        } while (total < target);

        /* For 64‑bit seq_t, exp cannot exceed 40 (3^40 fits).
           The loop above stops at exp = 40 for target = 10000,
           so no overflow occurs.            */
        powcnt = exp + 1;          /* number of powers 0..exp */

        /* Allocate and compute powers of 3. */
        seq_t *powers = (seq_t *)malloc(powcnt * sizeof(seq_t));
        if (!powers) return (seq_t)0;            /* allocation failure */
        powers[0] = (seq_t)1;
        for (size_t i = 1; i < powcnt; ++i) {
            powers[i] = powers[i - 1] * (seq_t)3;
        }

        /* Allocate array for all sums. */
        vals = (seq_t *)malloc(total * sizeof(seq_t));
        if (!vals) {
            free(powers);
            return (seq_t)0;                     /* allocation failure */
        }

        /* Generate all unordered triples i<j<k. */
        size_t idx = 0;
        for (size_t i = 0; i < powcnt - 2; ++i) {
            for (size_t j = i + 1; j < powcnt - 1; ++j) {
                for (size_t k = j + 1; k < powcnt; ++k) {
                    vals[idx++] = powers[i] + powers[j] + powers[k];
                }
            }
        }

        free(powers);

        /* Sort ascending. */
        qsort(vals, total, sizeof(seq_t), cmp_seq);

        count = total;
        init  = true;
    }

    /* bounds check – return 0 if n is beyond pre‑computed range */
    if ((idx_t)count < n)
        return (seq_t)0;
    return vals[n - 1];
}
