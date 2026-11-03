/*
 * A165113  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-9 Kaprekar map in A165110.
 * https://oeis.org/A165113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct terms in the trajectory of n under the
 *            * base‑9 Kaprekar map until the first repeated value. Each value
 *            * counted once: pre‑period length + cycle length
 * Range:     u8 n <= 82, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Validated: standard, pass, 121 terms through n = 6354 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Return the next value in the base‑9 Kaprekar map.  The input and
   output are of type seq_t. */
static seq_t kaprekar9(seq_t x)
{
    /* Working representation of the base‑9 digits of x.  The array is
       large enough to hold the digits of all numbers encountered in
       the trajectory for the ranges tested. */
    unsigned char digits[64];
    size_t len = 0;
    seq_t t = x;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t != 0) {
            digits[len++] = (unsigned char)(t % (seq_t)9);
            t /= (seq_t)9;
        }
    }

    /* Sort the digits in ascending order (insertion sort is
       sufficient because len is usually very small). */
    for (size_t i = 1; i < len; ++i) {
        size_t j = i;
        while (j > 0 && digits[j - 1] > digits[j]) {
            unsigned char tmp = digits[j - 1];
            digits[j - 1] = digits[j];
            digits[j] = tmp;
            --j;
        }
    }

    /* Construct the low and high numbers from the sorted digits. */
    seq_t low = 0;
    seq_t high = 0;
    const seq_t nine = (seq_t)9;

    for (size_t i = 0; i < len; ++i)
        low = low * nine + (seq_t)digits[i];

    for (size_t i = len; i > 0; --i)
        high = high * nine + (seq_t)digits[i - 1];

    return high - low;
}

/* Maximum number of distinct terms we expect to see in any trajectory
   for the ranges considered (far larger than needed). */
#define MAX_TRAJECTORY 2048

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* Store all distinct values encountered so far. */
    seq_t seen[MAX_TRAJECTORY];
    size_t m = 0;                /* number of distinct terms seen */

    seen[m++] = x;

    while (1) {
        seq_t next = kaprekar9(x);

        /* Does 'next' already appear in 'seen'? */
        for (size_t i = 0; i < m; ++i) {
            if (seen[i] == next) {
                return (seq_t)m;
            }
        }

        seen[m++] = next;
        x = next;
    }
}
