/*
 * A092632  Nonprime numbers with exactly two nonprime digits.
 * https://oeis.org/A092632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_nonprime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the OEIS definition: 
   a(n) is the n-th non‑prime integer that has exactly two digits
   belonging to {0,1,4,6,8,9}.  The function scans integers 
   from 10 upwards, checks compositeness and digit counts.*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_nonprime_digit.h"

/* Count the number of non‑prime digits in the base‑10 representation of n. */
static uint64_t count_nonprime_digits(uint64_t n) {
    uint64_t c = 0;
    do {
        int d = n % 10;
        if (is_nonprime_digit(d))
            c++;
    } while ((n /= 10));
    return c;
}

/* Simple deterministic trial division primality test for uint64_t values. 
   It is sufficient for the range that occurs for n ≤ 10 000. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = 5;
    int inc = 2;
    while (d * d <= n) {
        if (n % d == 0) return false;
        d += inc;
        inc = 6 - inc;  /* 5,7,11,13,17,19... */
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0)          /* offset is 1; index 0 is unused */
        return (seq_t)0;
    uint64_t found = 0;
    uint64_t num   = 10; /* first candidate: 10 */
    for (;;) {
        if (!is_prime_uint64(num) && count_nonprime_digits(num) == 2) {
            found++;
            if (found == n)
                return (seq_t)num;
        }
        num++;
    }
}
