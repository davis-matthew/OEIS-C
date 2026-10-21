/*
 * A084905  Smallest positive integer resulting after inserting division
 *          operator in the decimal string of n.
 * https://oeis.org/A084905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest quotient L/R obtainable by inserting a single
 *            division operator in the decimal representation of n. L and R
 *            are the numbers obtained by splitting the string of n between
 *            two consecutive digits; the division must be exact (R ≠ 0 and L
 *            % R == 0). If no such split exists, a(n) = n. a...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* the build supplies the definitions of idx_t and seq_t, e.g.:
   typedef uint64_t  idx_t;
   typedef uint64_t  seq_t;      (or an arbitrary‑precision type)     */
seq_t a(idx_t n)
{
    /* Special cases 0 and single‑digit numbers */
    if (n == 0) return (seq_t)0;
    idx_t tmp = n;
    int len = 0;
    while (tmp > 0) { tmp /= 10; ++len; }
    if (len <= 1) return (seq_t)n;

    /* Pre‑compute powers of ten up to the number of digits.
       len ≤ 19 for uint64_t n, so the array of size 21 is enough. */
    idx_t pow10[21];
    pow10[0] = 1;
    for (int i = 1; i <= len; ++i) pow10[i] = pow10[i - 1] * 10ULL;

    seq_t best = 0;      /* 0 will be a sentinel meaning “no valid split yet” */

    /* Iterate over all split positions:
       left part = first i digits, right part = remaining len-i digits */
    for (int i = 1; i < len; ++i) {
        int exp = len - i;          /* number of digits in the right part */
        idx_t divisor = pow10[exp]; /* 10^(len-i) */

        idx_t leftIdx  = n / divisor;
        idx_t rightIdx = n % divisor;
        if (rightIdx == 0) continue;   /* division by zero is illegal */

        seq_t left  = (seq_t)leftIdx;
        seq_t right = (seq_t)rightIdx;

        /* Check for an exact division */
        if (left % right == 0) {
            seq_t q = left / right;
            if (best == 0 || q < best) best = q;
        }
    }

    return (best == 0) ? (seq_t)n : best;
}
