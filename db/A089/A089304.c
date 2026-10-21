/*
 * A089304  Sum of all digits in all even numbers from 0 to 222...2 (with n
 *          2's).
 * https://oeis.org/A089304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of all digits in all even numbers from 0 to 222…2 (n 2's).
   The sequence is A089304.  The values are computed by a digit-DP
   that sums digits for all numbers in the range and then subtracts
   the sum for odd numbers.  The DP runs in O(n) time with only
   additions and small-multiplications on the type seq_t. */

#include <stdbool.h>

typedef struct { seq_t cnt; seq_t sum; } pair_t;

/* Compute both the count of numbers and the sum of their digits
   for all numbers from 0 to the n‑digit number consisting of all 2’s.
   If odd_last is true, the least significant digit is restricted
   to odd digits (1,3,5,7,9).  Otherwise all digits are allowed. */
static pair_t
compute_sum(idx_t n, bool odd_last)
{
    pair_t tight = {0, 0}, loose = {0, 0};
    tight.cnt = 1;                      /* start with empty prefix */

    for (idx_t pos = 0; pos < n; ++pos) {
        pair_t tight_next = {0, 0}, loose_next = {0, 0};

        /* Handle tight prefix (already equal to N up to pos-1) */
        if (tight.cnt != 0) {
            for (int d = 0; d <= 2; ++d) {
                /* Allow only odd digits if this is the last pos and odd_last */
                if (odd_last && pos + 1 == n && (d & 1) == 0)
                    continue;
                bool next_tight = (d == 2);
                pair_t *target = next_tight ? &tight_next : &loose_next;
                target->cnt  += tight.cnt;
                target->sum  += tight.sum + tight.cnt * (seq_t)d;
            }
        }

        /* Handle loose prefix (already less than N) */
        if (loose.cnt != 0) {
            if (odd_last && pos + 1 == n) {
                /* Only odd digits allowed at the last position */
                static const int odds[5] = {1,3,5,7,9};
                for (int k = 0; k < 5; ++k) {
                    int d = odds[k];
                    loose_next.cnt  += loose.cnt;
                    loose_next.sum  += loose.sum + loose.cnt * (seq_t)d;
                }
            } else {
                for (int d = 0; d <= 9; ++d) {
                    loose_next.cnt  += loose.cnt;
                    loose_next.sum  += loose.sum + loose.cnt * (seq_t)d;
                }
            }
        }

        tight = tight_next;
        loose = loose_next;
    }

    pair_t res = { tight.cnt + loose.cnt,
                   tight.sum + loose.sum };
    return res;
}

seq_t a(idx_t n)
{
    pair_t all = compute_sum(n, false);   /* sum of digits of all numbers 0..222…2 */
    pair_t odd = compute_sum(n, true);    /* sum of digits of odd numbers 0..222…2 */
    return all.sum - odd.sum;             /* sum for even numbers only */
}
