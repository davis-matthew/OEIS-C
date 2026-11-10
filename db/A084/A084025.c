/*
 * A084025  Divide n-th row of A084024 by n.
 * https://oeis.org/A084025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = (n-th term of A084024)/r where r is the row; A084024 row
 *            r = the r smallest r-digit palindromes divisible by r
 *            (0-padded). Computed by search over palindrome prefixes
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A084025_pow10(idx_t e) {
    seq_t res = 1;
    while (e) { res *= 10; e--; }
    return res;
}

static seq_t A084025_rev(seq_t v) {
    seq_t r = 0;
    while (v > 0) {
        r = r * 10 + v % 10;
        v /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = 1;
    idx_t start = 1; /* first index of row r */
    while (start + r <= n) {
        start += r;
        r++;
    }
    idx_t pos = n - start + 1;

    if (r % 10 == 0) return 0; /* palindrome cannot end in 0 */

    idx_t half_len = (r + 1) / 2;
    seq_t prefix = A084025_pow10(half_len - 1);
    seq_t prefix_max = A084025_pow10(half_len) - 1;
    seq_t r_seq = (seq_t)r;
    seq_t count = 0;
    seq_t p10 = A084025_pow10(half_len);
    seq_t p10m = A084025_pow10(half_len - 1);

    while (prefix <= prefix_max) {
        seq_t pal;
        if (r % 2 == 0)
            pal = prefix * p10 + A084025_rev(prefix);
        else
            pal = prefix * p10m + A084025_rev(prefix / 10);
        if (pal % r_seq == 0) {
            ++count;
            if (count == (seq_t)pos)
                return pal / r_seq;
        }
        prefix = prefix + 1;
    }
    return 0;
}
