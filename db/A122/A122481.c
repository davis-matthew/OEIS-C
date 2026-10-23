/*
 * A122481  a(n) = if n < 10 then n else a(digitsum(n)) + digitsum(n), where
 *          digitsum(n) = A007953(n), the sum of digits in decimal
 *          representation of n.
 * https://oeis.org/A122481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n < 10 then n else a(digitsum(n)) + digitsum(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digitsum(n) is the sum of decimal digits of n. */

#include <stdint.h>          /* for uintptr_t, if needed */
#include <stddef.h>          /* for size_t (not used but harmless) */

/* `idx_t` is an unsigned 64‑bit integer type, `seq_t` is an unsigned
   integer or arbitrary‑precision integer type as supplied by the
   build.  The helper below computes the decimal digit sum of an
   `idx_t` value. */

static inline seq_t digit_sum_idx(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += (seq_t)(n % 10);
        n /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 10) {
        return (seq_t)n;          /* base case for single‑digit numbers */
    }
    seq_t ds = digit_sum_idx(n);  /* digit sum of n */
    return a((idx_t)ds) + ds;     /* recursive call with the digit sum */
}
