/*
 * A044538  Numbers n such that string 2,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A044538
 *  a(n) = n-th positive integer such that the base‑7 representation of n
 *  contains the substring 25, but the base‑7 representation of n+1 does not.
 *  Offset: 1
 */

#include <stdbool.h>

/* Return true if the base‑7 digits of x contain the pattern 2 followed by 5.
 * Digits are examined from most significant to least.  The function
 * works for any seq_t capable of modulo, division, comparison, and
 * addition.
 */
static bool has25(seq_t x)
{
    if (x == 0)
        return false;

    /* 128 digits is far more than enough for all values we generate
     * even for very large arbitrary‑precision seq_t during the test
     * range.  The array holds digits from least significant to most.   */
    int digits[128];
    int len = 0;
    seq_t t = x;

    while (t > 0) {
        digits[len++] = (int)(t % (seq_t)7);
        t /= (seq_t)7;
    }

    /* Check for the pattern 2 (higher) followed by 5 (lower).        */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 5)
            return true;
    }
    return false;
}

/*  Main sequence function.  It is not memoised because the sequence
 *  grows only linearly for the required input range.  The complexity
 *  is O(n ⋅ log n) with a very small constant.
 */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for offset < 1, guard clause */
        return (seq_t)0;

    idx_t  count = 0;
    seq_t  x     = 1;   /* start from the first positive integer */

    while (count < n) {
        if (has25(x) && !has25(x + 1)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
    return (seq_t)0;    /* unreachable, keeps the compiler happy */
}
