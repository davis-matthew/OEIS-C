/*
 * A103849  Rearrangement of natural numbers according to the sums of odd and
 *          even digits as partitioned by A036301, A103848 and A103829 (see
 *          Comments).
 * https://oeis.org/A103849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A103849: Rearrangement according to A036301, A103848, A103829.
   Formula:  a(1) = 0;
             for k ≥ 1:
               a(3k-1) = A036301(k+1),
               a(3k  ) = A103848(k),
               a(3k+1) = A103829(k).
   The three auxiliary sequences are defined by the sums of even and
   odd decimal digits of a natural number:  equal, less, or greater. */

#include <stdint.h>

/* return  0 if sum_even == sum_odd
 *        -1 if sum_even < sum_odd
 *         1 if sum_even > sum_odd   */
static int digit_sum_even_odd(idx_t x, idx_t *sum_even, idx_t *sum_odd)
{
    *sum_even = 0;
    *sum_odd  = 0;
    if (x == 0) {
        /* 0 has equal sums */
        return 0;
    }
    while (x) {
        int d = (int)(x % 10);
        if ((d & 1) == 0) {
            *sum_even += d;
        } else {
            *sum_odd += d;
        }
        x /= 10;
    }
    if (*sum_even < *sum_odd) return -1;
    if (*sum_even > *sum_odd) return 1;
    return 0;
}

/* return k-th number (1‑based) whose digit‑sum relation equals 'rel'
 * rel = 0: equal
 *   =-1: even < odd
 *   = 1: even > odd
 * The sequences start with 0 for the equal case; other cases start at 1. */
static seq_t get_kth_by_relation(idx_t k, int rel)
{
    idx_t count = 0;
    idx_t n;
    if (rel == 0)
        n = 0;
    else
        n = 1;

    while (1) {
        idx_t se, so;
        int cmp = digit_sum_even_odd(n, &se, &so);
        if (cmp == rel) {
            ++count;
            if (count == k)
                return (seq_t)n;   /* cast/construct to seq_t */
        }
        ++n;
    }
}

/* main sequence function */
extern seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;           /* offset 1 */

    idx_t t   = n - 2;              /* 0‑based position after the first term */
    idx_t r   = t % 3;              /* 0 → equal, 1 → less, 2 → greater */
    idx_t base= t / 3 + 1;          /* base index for k */

    if (r == 0) {                  /* A036301(k+1) with k = base+1 */
        return get_kth_by_relation(base + 1, 0);
    } else if (r == 1) {            /* A103848(k) with k = base   */
        return get_kth_by_relation(base, -1);
    } else {                       /* r == 2: A103829(k) with k = base */
        return get_kth_by_relation(base, 1);
    }
}
