/*
 * A101412  Least number of odd squares that sum to n.
 * https://oeis.org/A101412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Least number of odd squares that sum to n, defined by case
 *            analysis on n mod 8 (OEIS A101412)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 7892 (2026-11-01)
 * Helpers:   is_square, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_square.h"
#include "../utils/isqrt.h"

/* Test whether a number n (seq_t) can be expressed as a sum of two odd
   squares.  */
static bool has_two_odd_square_sum(seq_t n)
{
    idx_t nn = (idx_t)n;                /* safe for n ≤ 2^64‑1 */
    idx_t sqrt_n = isqrt(nn);

    for (idx_t a = 1; a <= sqrt_n; a += 2) {
        idx_t a2 = a * a;
        if (a2 > nn) break;
        idx_t rem = nn - a2;
        idx_t b = isqrt(rem);
        if (b * b == rem && (b & 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index 0 unused */

    idx_t mod8 = n & 7;                 /* n % 8 */

    /* n ≡ 1 (mod 8) */
    if (mod8 == 1) {
        return (seq_t)(is_square((seq_t)n) ? 1 : 9);
    }

    /* n ≡ 2 (mod 8) */
    if (mod8 == 2) {
        if (n % 72 == 42 || n % 72 == 66)
            return (seq_t)10;
        return (seq_t)(has_two_odd_square_sum((seq_t)n) ? 2 : 10);
    }

    /* Remaining residues */
    switch (mod8) {
        case 3: return (seq_t)3;
        case 4: return (seq_t)4;
        case 5: return (seq_t)5;
        case 6: return (seq_t)6;
        case 7: return (seq_t)7;
        case 0: return (seq_t)8;
        default: return (seq_t)0;     /* unreachable */
    }
}
