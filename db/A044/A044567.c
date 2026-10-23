/*
 * A044567  Numbers n such that string 6,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose base‑7 representation * contains
 *            the substring “66” and the next integer has no such * substring.
 *            The function searches numbers sequentially, testing * the base‑7
 *            representation for the required pattern
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_valid(seq_t x)
{
    /* convert x to base‑7 digits, least significant first */
    unsigned int digits[64];
    seq_t tmp = x;
    int len = 0;
    while (tmp > 0) {
        digits[len++] = (unsigned int)(tmp % 7);
        tmp /= 7;
    }
    if (len < 2)               /* need at least one “66” pair */
        return false;

    /* count trailing 6’s */
    int i = 0;
    while (i < len && digits[i] == 6)
        ++i;
    if (i < 2)                 /* at least two trailing 6’s */
        return false;

    if (i == len)              /* entire number is only 6’s */
        return true;

    /* ensure the prefix (digits i … len‑1) contains no “66” */
    for (int j = i + 1; j < len; ++j)
        if (digits[j] == 6 && digits[j - 1] == 6)
            return false;

    /* prevent a new “66” created at the boundary after increment:
       if the digit left of the trailing run is 5, it becomes 6 after
       increment; it would form “66” with the previous digit only if
       that digit is 6. */
    if (digits[i] == 5) {
        if (i + 1 < len && digits[i + 1] == 6)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = 48; /* first term in the sequence */
    for (;;) {
        if (is_valid(num)) {
            ++found;
            if (found == n)
                return num;
        }
        num = num + 1;
    }
}
