/*
 * A050745  Numbers k such that the decimal expansion of k^6 contains no pair
 *          of consecutive equal digits.
 * https://oeis.org/A050745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k (a(1)=0) such that the decimal
 *            expansion of k^6 contains no pair of consecutive equal digits.
 *            The function searches forward from 0 and stops when the n‑th
 *            such k is found. This works for any seq_t that supports the
 *            arithmetic and comparison operators
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 18, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* test whether a seq_t number has no consecutive equal decimal digits */
static inline bool no_consecutive_equal_digits(seq_t v)
{
    /* 0 is allowed and has no consecutive digits */
    if (v <= 0) return true;

    int prev = -1;                     /* sentinel for “no previous digit” */
    const seq_t ten = (seq_t)10;       /* 10 as a seq_t value */

    while (v > 0) {
        int digit = (int)(v % ten);    /* least‑significant decimal digit */
        if (digit == prev) return false;
        prev = digit;
        v = v / ten;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* For safety: the specification starts at offset 1 (a(1)=0). */
    if (n == 0) return 0;

    seq_t k = 0;                  /* candidate k */
    idx_t count = 0;              /* number of valid k’s found so far */

    while (1) {
        seq_t k3 = k * k * k;     /* k³ */
        seq_t k6 = k3 * k3;       /* k⁶ */

        if (no_consecutive_equal_digits(k6)) {
            count++;
            if (count == n) return k;
        }
        k = k + 1;
    }
}
