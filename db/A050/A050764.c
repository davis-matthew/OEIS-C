/*
 * A050764  Numbers of form k^k (for values of k see A050763) containing no
 *          pair of consecutive equal digits (probably finite).
 * https://oeis.org/A050764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the k-th integer k^k such that k^k contains no pair of
 *            consecutive equal digits (computed by a simple sequential
 *            search)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

static seq_t pow_k_to_k(idx_t k)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)k;
    idx_t exp    = k;
    while (exp)
    {
        if (exp & (idx_t)1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Sequence starts at offset 1 */
    if (n == (idx_t)0) return (seq_t)0;
    idx_t k       = (idx_t)1;
    idx_t counter = (idx_t)0;
    while (1)
    {
        seq_t val = pow_k_to_k(k);
        if (no_consecutive_equal_digits(val))
        {
            counter++;
            if (counter == n) return val;
        }
        k++;
    }
}
