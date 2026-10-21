/*
 * A216481  a(n) is the least multiple of n which uses only digit 2, or a(n) =
 *          -1 if no such multiple exists.
 * https://oeis.org/A216481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 16, i64 n <= 22, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Least multiple of n made of only the digit 2.
    If no such multiple exists the function returns -1.
    The number 222…(k times) equals 2·(10^k – 1)/9.
    So we find the smallest k such that the repunit Rk = (10^k – 1)/9
    is divisible by n / gcd(n,2).  A repunit can be divisible by a
    number only if that number is coprime with 10; otherwise the
    answer is –1. */

/*  Returns a(n) for n ≥ 1.  The type seq_t may be a signed integer
    of various sizes or an arbitrary‑precision integer; index n is
    of type idx_t.  No I/O operations are performed.  The function
    is thread‑safe and handles calls in any order. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle trivial history */
    if (n == 0) {          /* offset is 1; 0 is undefined, return -1 */
        return (seq_t)-1;
    }

    /* gcd(n,2) is 1 if n odd, 2 if n even */
    idx_t g = (n & 1) ? 1 : 2;
    idx_t m = n / g;

    /* A repunit exists only if m is coprime with 10 */
    if ((m % 2 == 0) || (m % 5 == 0)) {
        return (seq_t)-1;
    }

    /* Find the length of the shortest repunit divisible by m */
    idx_t rem = 0;
    idx_t k   = 0;
    do {
        ++k;
        rem = (rem * 10 + 1) % m;
    } while (rem != 0);

    /* Build the number consisting of k digits '2' */
    seq_t res = 0;
    for (idx_t i = 0; i < k; ++i) {
        res = res * 10 + 2;
    }
    return res;
}
