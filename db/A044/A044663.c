/*
 * A044663  Numbers n such that string 3,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th number such that its base‑9 representation
 *            contains the substring “34” but the base‑9 representation of the
 *            following integer does not contain “34”. The sequence begins 31,
 *            112, 193, …
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the nth element of the sequence
   for any n >= 1.  It performs a simple brute force search, testing
   successive integers for the required property. */

#include <stdbool.h>

static bool has34(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        int digit = (int)(x % 9);
        if (!first && prev == 4 && digit == 3)
            return true;
        first = false;
        prev = digit;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1, any value */
        return 0;

    idx_t count = 0;
    seq_t num = 1;

    for (;;) {
        if (has34(num) && !has34(num + 1)) {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
