/*
 * A050812  Number of times n is palindromic in bases b, 2 <= b <= 10.
 * https://oeis.org/A050812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of bases 2 ≤ b ≤ 10 in which n is a palindrome. */
#include <stdbool.h>

static bool is_pal_in_base(seq_t n, int base)
{
    if (n == 0)
        return true;                     /* 0 is a palindrome in any base */

    /* For 64‑bit numbers the maximum number of base‑2 digits is 64.
       If seq_t is larger, the array has enough room for most cases
       (256 digits covers huge values; dynamic allocation is avoided
       for speed). */
    int digits[256];
    int len = 0;

    while (n > 0) {
        digits[len++] = (int)(n % base);
        n = n / base;
    }

    for (int i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i])
            return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (int base = 2; base <= 10; ++base)
        if (is_pal_in_base(n, base))
            ++count;
    return count;
}
