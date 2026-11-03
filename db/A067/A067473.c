/*
 * A067473  Smallest n-digit square starting with 3.
 * https://oeis.org/A067473
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ceil(sqrt(3*10^(n-1)))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Ceil(sqrt(v)) for uint64_t or arbitrary‑precision seq_t.
   Returns the smallest integer k such that k^2 ≥ v. */
static inline seq_t ceil_sqrt_seq(seq_t v)
{
    if (v == (seq_t)0) return (seq_t)0;
    seq_t lo = (seq_t)1, hi = v, ans = v;
    while (lo <= hi) {
        seq_t mid = (lo + hi) / (seq_t)2;
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + (seq_t)1;
        } else {
            ans = mid;
            if (mid == (seq_t)0) break;
            hi = mid - (seq_t)1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined for n < 2 */
    seq_t p10 = pow10(n - 1);        /* 10^(n-1) */
    seq_t t   = p10 * ((seq_t)3);    /* 3*10^(n-1) */
    seq_t r   = ceil_sqrt_seq(t);    /* ceil(sqrt(t)) */
    return r * r;                    /* square it */
}
