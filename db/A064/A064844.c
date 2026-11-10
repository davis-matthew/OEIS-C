/*
 * A064844  Number of iterations of x -> x + A064834(x) to reach a palindrome,
 *          starting with n (or -1 if no palindrome is ever reached).
 * https://oeis.org/A064844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of steps of x -> x + A064834(x) from x = n until
 *            A064834(x) = 0 (x is a palindrome); A064834(x) =
 *            Sum_{i=1..floor(k/2)} |d_i - d_{k-i+1}| over the decimal digits
 *            of x. Found by direct iteration
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* A064834 on a seq_t value */
static seq_t A064844_dev(seq_t x)
{
    unsigned digits[4096];
    size_t len = 0;
    seq_t res = 0;
    if (x == 0) return 0;
    while (x != 0 && len < 4096) {
        digits[len++] = (unsigned)(long long)(x % 10);
        x /= 10;
    }
    size_t half = len / 2;
    for (size_t i = 0; i < half; ++i) {
        unsigned d1 = digits[i];
        unsigned d2 = digits[len - 1 - i];
        res += (seq_t)(d1 > d2 ? d1 - d2 : d2 - d1);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t count = 0;
    long long iter;
    for (iter = 0; iter < 5000000LL; ++iter) {
        seq_t s = A064844_dev(x);
        if (s == 0) return count;
        x += s;
        ++count;
    }
    return (seq_t)0 - (seq_t)1; /* no palindrome reached (-1) */
}
