/*
 * A158030  Triangular numbers t such that all the digits needed to write the
 *          consecutive triangular numbers from 0 to t fill exactly an
 *          equilateral triangle (no holes, no overlaps).
 * https://oeis.org/A158030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A158030.
   The sequence consists of the triangular numbers t such that
   the total number of decimal digits needed to write all triangular
   numbers from 0 up to t (inclusive) is itself a triangular number. */

#include <stdint.h>
#include <stdbool.h>

/* --------------------- helper functions --------------------- */

/* Count decimal digits of a non‑negative seq_t value.
   Works for any integer type supporting the usual operators. */
static uint64_t digit_len_seq(seq_t x)
{
    if (x == 0) return 1;
    uint64_t cnt = 0;
    seq_t ten = (seq_t)10;
    while (x > 0) {
        x /= ten;
        ++cnt;
    }
    return cnt;
}

/* Integer square root of a 64‑bit unsigned integer.
   Sn: val ≤ 2^64−1.  Uses binary search and division to avoid overflow. */
static uint64_t isqrt_uint64(uint64_t val)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid <= val / mid) {               /* mid*mid ≤ val  (no overflow) */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Test whether a 64‑bit unsigned integer y is triangular.
   y is triangular iff 8*y+1 is a perfect square. */
static bool is_triangular_uint64(uint64_t y)
{
    uint64_t val = 8 * y + 1;
    uint64_t s   = isqrt_uint64(val);
    return s * s == val;
}

/* ------------------------- main function --------------------- */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* index 0 is outside the prescribed offset
                               but we return 0 to stay safe. */ 
        return (seq_t)0;
    }

    idx_t          m        = 0;   /* index of current triangular number */
    seq_t          t        = (seq_t)0;   /* T_m = m*(m+1)/2 */
    uint64_t       sum     = 0;   /* running sum of digit counts */
    idx_t          found   = 0;   /* how many qualifying t have been found */

    while (found < n) {
        /* compute digit count of current triangular number */
        uint64_t d = digit_len_seq(t);

        /* update cumulative digit count */
        sum += d;

        /* test if cumulative count is triangular */
        if (is_triangular_uint64(sum)) {
            ++found;
            if (found == n) {
                return t;
            }
        }

        /* advance to next triangular number */
        ++m;
        t += (seq_t)m;   /* T_(m) = T_(m-1) + m */
    }

    /* control should never reach here as we return inside the loop */
    return t;
}
