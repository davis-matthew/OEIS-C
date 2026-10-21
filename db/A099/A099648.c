/*
 * A099648  Largest term arising in complete-iteration-list (both transient
 *          and cycle) when f(x) = A003132(x) is iterated, i.e., if
 *          digit-squares of iterate added repeatedly until steady state (=
 *          either cycle or fixed point) is reached.
 * https://oeis.org/A099648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the largest value that appears during the iteration
   of the digit‑square‑sum function f(x) until a previously seen value occurs.
   For each index n (starting at 1) it returns a(n).  The algorithm is a
   direct simulation: it keeps the current value, records the greatest
   value seen, and stops when the current value repeats.  Because the
   first iteration reduces any n to at most 1620, only the initial n can
   ever be larger than 2000, so a small visited array suffices. */

#include <stdbool.h>

static seq_t digit_square_sum(uint64_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        unsigned d = x % 10;
        sum += (seq_t)d * (seq_t)d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t max_val = (seq_t)n;                     /* current maximum          */
    seq_t cur = digit_square_sum((uint64_t)max_val);   /* first iterate       */

    /* Mark whether an intermediate value has been seen.
       2000 is safely larger than any possible value after the first step. */
    enum { VISIT_SIZE = 2000 };
    bool visited[VISIT_SIZE] = { false };

    if ((uint64_t)max_val < VISIT_SIZE)
        visited[(int)(uint64_t)max_val] = true;

    for (;;) {
        if ((uint64_t)cur < VISIT_SIZE) {
            if (visited[(int)(uint64_t)cur])
                break;
            visited[(int)(uint64_t)cur] = true;
        } else { /* cur >= 2000 – only possible if cur == n */
            if (cur == max_val)
                break;
        }

        if (cur > max_val)                /* keep track of the maximum   */
            max_val = cur;

        cur = digit_square_sum((uint64_t)cur);
    }

    return max_val;
}
