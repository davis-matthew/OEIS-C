/*
 * A121030  Multiples of 10 containing a 10 in their decimal representation.
 * https://oeis.org/A121030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9783 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th multiple of 10 that contains
   the substring "10" in its decimal representation
   (by brute‑force search). */

#include <stdbool.h>

static bool contains10(seq_t x)
{
    seq_t tmp = x;
    seq_t prev = tmp % 10;   /* least significant digit */
    tmp = tmp / 10;

    while (tmp > 0) {
        seq_t cur = tmp % 10;
        if ((long long)cur == 1 && (long long)prev == 0)
            return true;
        prev = cur;
        tmp = tmp / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t k = 0;              /* count of found terms */
    idx_t i = 1;              /* multiplier for 10 */

    while (k < n) {
        seq_t candidate = (seq_t)i * 10;
        if (contains10(candidate)) {
            ++k;
            if (k == n)
                return candidate;
        }
        ++i;
    }
    return 0; /* unreachable */
}
