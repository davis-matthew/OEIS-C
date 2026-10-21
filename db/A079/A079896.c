/*
 * A079896  Discriminants of indefinite binary quadratic forms.
 * https://oeis.org/A079896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer that is ≡0 or ≡1 (mod 4) and is
 *            not a perfect square. a(1)=5, a(2)=8, a(3)=12, …
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff x is a perfect square.  Works for any integer type
   seq_t that supports + - * / % and comparisons. */
static bool is_square(seq_t x)
{
    /* 0 and 1 are perfect squares. */
    if (x <= (seq_t)1)
        return true;

    /* Binary search for floor(sqrt(x)). */
    seq_t low  = 1;
    seq_t high = x >> 1;
    if (high < 1) high = 1;

    while (low <= high) {
        seq_t mid  = low + (high - low) / 2;
        seq_t div  = x / mid;           /* mid*mid ≤ x  ⇔  mid ≤ x/mid   */
        if (mid <= div)
            low = mid + 1;
        else
            high = mid - 1;
    }

    seq_t r = high;                     /* floor(sqrt(x))  */
    return (r * r == x);
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many terms found so far */
    seq_t candidate = 1;     /* start at 1 */

    while (found < n) {
        /* Check congruence modulo 4. */
        seq_t mod4 = candidate % (seq_t)4;
        if (mod4 == 0 || mod4 == 1) {
            /* Skip perfect squares. */
            if (!is_square(candidate)) {
                ++found;
                if (found == n)
                    return candidate;
            }
        }
        ++candidate;
    }

    /* Should never be reached. */
    return (seq_t)0;
}
