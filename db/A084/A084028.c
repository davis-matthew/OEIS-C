/*
 * A084028  Average of n-th row of A084024.
 * https://oeis.org/A084028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A084027(n)/n = mean of row n of A084024, where row n
 *            holds the n smallest n-digit palindromes divisible by n, padded
 *            with 0. The code finds them by searching palindrome prefixes in
 *            increasing order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"

static seq_t A084028_p10(idx_t e) {
    seq_t r = 1;
    while (e) { r *= 10; e--; }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n % 10 == 0) return 0;          /* palindrome would start with 0 */
    if (n % 25 == 0 && n >= 2) return 0; /* last two digits equal: never 00/25/50/75 */

    idx_t half = (n + 1) / 2;
    seq_t pmin = A084028_p10(half - 1);
    seq_t pmax = A084028_p10(half) - 1;
    seq_t start = pmin;
    if (n % 2 == 0) start = pmin * 2;
    else if (n % 5 == 0) start = pmin * 5;

    seq_t pw_h = A084028_p10(half);
    seq_t pw_h1 = A084028_p10(half - 1);
    seq_t N = (seq_t)n;
    seq_t sq = 0, sr = 0;
    idx_t count = 0;

    for (seq_t prefix = start; prefix <= pmax && count < n; prefix++) {
        seq_t pal;
        if (n % 2 == 0)
            pal = prefix * pw_h + reverse_digits(prefix);
        else
            pal = prefix * pw_h1 + reverse_digits(prefix / 10);
        if (pal % N == 0) {
            count++;
            sq += pal / N;
            sr += pal % N;
        }
    }
    return sq + sr / N;
}
