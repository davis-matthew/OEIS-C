/*
 * A072139  Last term of the preperiodic part of the 'Reverse and Subtract'
 *          trajectory of n, or -1 if the trajectory is completely periodic.
 * https://oeis.org/A072139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A072139 by Floyd cycle detection on the
   Reverse‑and‑Subtract sequence.  The function returns the value
   that precedes the first repeated value in the trajectory of n;
   returns -1 if n immediately lies on a cycle. */

#include <stdbool.h>
#include <stddef.h>

/* Reverse the decimal digits of a non‑negative seq_t value. */
static seq_t rev_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x   = x / (seq_t)10;
    }
    return rev;
}

/* f(x) = |x – reverse(x)| */
static seq_t reverse_subtract(seq_t x)
{
    seq_t r = rev_digits(x);
    return (x >= r) ? (x - r) : (r - x);
}

seq_t a(idx_t n)
{
    /* Floyd's cycle detection */
    seq_t tortoise = reverse_subtract((seq_t)n);               /* f(x0)    */
    seq_t hare     = reverse_subtract(tortoise);              /* f(f(x0)) */

    while (tortoise != hare) {
        tortoise = reverse_subtract(tortoise);
        hare = reverse_subtract(reverse_subtract(hare));
    }

    /* They now meet at the first repeated element inside the cycle. */
    seq_t meeting = tortoise;

    /* Find the starting point of the cycle (mu). */
    seq_t tortoise_start = (seq_t)n;
    seq_t hare_start = meeting;
    size_t mu = 0;
    while (tortoise_start != hare_start) {
        tortoise_start = reverse_subtract(tortoise_start);
        hare_start = reverse_subtract(hare_start);
        ++mu;
    }

    /* If mu == 0, the starting number is already part of the cycle. */
    if (mu == 0)
        return -(seq_t)1;   /* -1 */

    /* The last preperiodic element is the one just before the cycle starts. */
    seq_t prev = (seq_t)n;
    seq_t cur  = reverse_subtract(prev);
    for (size_t step = 1; step < mu; ++step) {
        prev = cur;
        cur  = reverse_subtract(cur);
    }
    return prev;
}
