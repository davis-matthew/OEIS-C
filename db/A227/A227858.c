/*
 * A227858  Numbers which start and end with the same digit in decimal.
 * https://oeis.org/A227858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 4991, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A227858: the n‑th integer whose decimal
   representation starts and ends with the same digit.
   An upper bound of 10*n + 1000 suffices for all tested values; if not,
   the bound is doubled until it contains the n‑th term.  The search is
   performed by a binary search on this bound using the helper
   count_leq() that counts numbers ≤ x with the required property. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x);          */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e);            */

/* Count how many integers ≤ x have the same first and last decimal digit. */
static seq_t count_leq(seq_t x)
{
    if (x == 0) return 0;

    int D = digit_len(x);                 /* number of decimal digits */
    if (D == 1) return x;                 /* all 1‑digit numbers satisfy */

    seq_t powD2 = pow10((idx_t)(D - 2));   /* 10^(D-2) */
    seq_t total = powD2 + 8;               /* numbers of length < D  */

    seq_t powD1 = pow10((idx_t)(D - 1));   /* 10^(D-1) */
    idx_t f0 = (idx_t)(x / powD1);         /* first decimal digit (1..9) */

    /* middle part (digits 2 .. D-1)   */
    seq_t midx  = (x - (seq_t)f0 * powD1) / 10;
    seq_t last  = x % 10;                  /* last decimal digit */

    /* numbers with first digit < f0 and length D */
    if (f0 > 1) {
        total += powD2 * (seq_t)(f0 - 1);
    }

    /* numbers with first digit == f0, last digit == f0, and length D */
    if (!(midx == 0 && last < (seq_t)f0)) {
        seq_t tmp  = midx * 10 + last - (seq_t)f0;
        seq_t cnt  = tmp / 10 + 1;         /* floor(tmp/10)+1 */
        total += cnt;
    }

    return total;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t low    = 1;
    seq_t high   = (seq_t)n * 10 + 1000;   /* safe upper bound for n ≤ 10000 */

    /* Enlarge upper bound until it contains the n‑th term. */
    while (count_leq(high) < target) {
        high = high * 2;
    }

    /* Binary search for smallest x with count_leq(x) ≥ target. */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        if (count_leq(mid) < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
