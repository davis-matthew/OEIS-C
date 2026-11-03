/*
 * A077366  Triangular numbers whose internal digits form a triangular number.
 *          Or triangular number such that deleting the MSD and LSD leaves a
 *          triangular number.
 * https://oeis.org/A077366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   num_digits_seq_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077366: Triangular numbers whose internal digits form a triangular number */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_seq_to_u32.h"

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Return 10^exp as a seq_t.  exp is a non‑negative integer. */
static seq_t seq_pow10(unsigned int exp)
{
    seq_t v = 1;
    while (exp-- > 0)
        v *= 10;
    return v;
}

/* Return the integer represented by the interior decimal digits of v
   (i.e., after deleting the most and least significant digits).
   For 0 ≤ v < 10, the result is 0. */
static seq_t interior_digits(seq_t v)
{
    unsigned int d = num_digits_seq_to_u32(v);
    if (d <= 2)
        return 0;
    seq_t mod = seq_pow10(d - 2);
    return (v / 10) % mod;
}

/* Return true iff x is a triangular number (x = m(m+1)/2 for some m). */
static bool is_triangular(seq_t x)
{
    seq_t v = 8 * x + 1;
    seq_t s = isqrt_seq(v);
    return s * s == v;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t tri = 0;           /* T_0 = 0 */
    for (idx_t k = 0;; ++k)
    {
        if (is_triangular(interior_digits(tri)))
        {
            ++count;
            if (count == n)
                return tri;
        }
        tri += (k + 1);      /* T_{k+1} = T_k + (k+1) */
    }
}
