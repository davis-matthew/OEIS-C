/*
 * A064704  Numbers beginning and ending with their multiplicative digital
 *          root.
 * https://oeis.org/A064704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   A064704 – numbers beginning and ending with their multiplicative
 *            digital root
 * Range:     u8 n <= 13, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 848 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: compute the multiplicative digital root of a 64‑bit integer.
   Assumes the product of digits never exceeds 64 bits for all numbers
   encountered during the required range. */
static unsigned long long mul_digital_root(unsigned long long n) {
    while (n >= 10) {
        unsigned __int128 prod = 1;
        unsigned long long t = n;
        while (t > 0) {
            prod *= t % 10;
            t /= 10;
        }
        n = (unsigned long long)prod;  /* truncation safe under assumptions */
    }
    return n;
}

/* Test if x satisfies: first digit == last digit == multiplicative digital root */
static bool is_valid(uint64_t x) {
    if (x == 0) return false;   /* leading zero impossible */
    uint8_t first = 0, last = (uint8_t)(x % 10);
    uint64_t t = x;
    while (t >= 10) t /= 10;
    first = (uint8_t)t;
    if (first == 0 || first != last) return false;
    return mul_digital_root(x) == (unsigned long long)first;
}

/* Main sequence function.  Generates terms on the fly by brute force search,
   which is fast enough for n up to about 10 000. */
seq_t a(idx_t n) {
    idx_t count = 0;
    for (uint64_t cand = 1; ; ++cand) {
        if (is_valid(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
    }
}
