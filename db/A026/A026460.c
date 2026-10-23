/*
 * A026460  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-2 for
 *          1<=i<=j<=n, n >= 2.
 * https://oeis.org/A026460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) such that a(n) ≠
 *            a(i)*a(j)-2 for all 1 ≤ i ≤ j ≤ n
 * Range:     u8 n <= 141, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

/* linear search for a value in a sorted array */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] == val)
            return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

static seq_t *a_seq = NULL;   /* stored terms a(1), a(2), … */
static size_t a_len = 0;      /* number of terms already computed */
static size_t a_cap = 0;      /* allocated capacity of a_seq */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1 */

    /* initialise with first term */
    if (a_len == 0) {
        grow_seq(&a_seq, &a_cap, 1);
        a_seq[0] = (seq_t)1;
        a_len = 1;
    }

    /* compute terms up to requested index */
    while (a_len < (size_t)n) {
        seq_t cand = a_seq[a_len - 1] + (seq_t)1;
        /* candidate must be > previous term */
        while (true) {
            seq_t t = cand + (seq_t)2;   /* we need cand+2 == a(i)*a(j) */
            bool forbidden = false;

            /* check all existing terms a(1)...a(n-1) as possible a(i) */
            for (size_t i = 0; i < a_len; ++i) {
                seq_t ai = a_seq[i];
                if (ai > t)
                    break;                 /* no further divisors possible */

                if (t % ai != (seq_t)0)
                    continue;

                seq_t aj = t / ai;
                if (contains(a_seq, a_len, aj)) {
                    forbidden = true;
                    break;
                }
            }

            /* special case: cand itself may appear twice (i=j=cand) */
            if (!forbidden && cand == (seq_t)2)
                forbidden = true;  /* because 2*2-2 = 2 */

            if (!forbidden)
                break;   /* cand found */

            ++cand;   /* try next candidate */
        }

        /* store the found term */
        grow_seq(&a_seq, &a_cap, a_len + 1);
        a_seq[a_len] = cand;
        ++a_len;
    }

    return a_seq[n - 1];
}
