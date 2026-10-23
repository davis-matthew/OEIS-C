/*
 * A026443  a(n) = least positive integer > a(n-1) and not a(i)*a(j)+2 for
 *          1<=i<j<=n, with initial terms 1,2.
 * https://oeis.org/A026443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer > a(n-1) that is not equal to * a(i)*a(j)+2
 *            for any 1 <= i < j <= n-1. * Initial values: a(1)=1, a(2)=2. * *
 *            The routine maintains the sequence in a static array and * uses
 *            a brute‑force test for each candidate. The test is * O(n log n)
 *            per candidate (loop over i, binary sear...
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"

static seq_t *vals      = NULL;   /* array of sequence values          */
static size_t len       = 0;       /* number of values stored           */
static size_t cap       = 0;       /* array capacity                    */

/* initialise the first two terms if needed --------------------------------*/
static void init_sequence(void)
{
    if (len >= 2) return;              /* already initialised */

    /* ensure enough capacity	for 2 elements */
    if (cap < 2) grow_seq_array(&vals, &cap, 2);

    vals[0] = (seq_t)1;
    vals[1] = (seq_t)2;
    len    = 2;
}

/* binary search for x in vals[0..len-1]; returns index or len if not found */
static size_t binary_search(seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (vals[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined; return 0 as safe value */

    init_sequence();                    /* make sure a[1]=1, a[2]=2 exist */

    while (len < (size_t)n) {           /* generate terms up to requested n   */
        seq_t cand = vals[len - 1] + (seq_t)1;
        for (;;) {
            seq_t y = cand - (seq_t)2;   /* a(i)*a(j) = y   */
            bool forbidden = false;

            for (size_t i = 0; i < len; ++i) {
                seq_t ai = vals[i];
                if (ai > y) break;      /* further ai cannot divide y */

                if (y % ai == 0) {
                    seq_t q = y / ai;   /* second factor = q   */
                    if (q <= ai) continue;   /* require i < j */

                    size_t idx = binary_search(q);
                    if (idx < len && vals[idx] == q && idx > i) {
                        forbidden = true;
                        break;
                    }
                }
            }
            if (!forbidden) break;       /* suitable candidate found   */
            ++cand;                      /* try the next integer       */
        }

        /* store the new term */
        if (len >= cap) grow_seq_array(&vals, &cap, cap * 2);
        vals[len++] = cand;
    }

    return vals[n - 1];
}
