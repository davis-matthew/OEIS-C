/*
 * A171492  Numbers that are not divisible by each of their nonzero decimal
 *          digits.
 * https://oeis.org/A171492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8631 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer that is NOT divisible by
   each of its nonzero decimal digits. This implementation tests each
   integer successively and counts those that satisfy the condition. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true if integer num has at least one nonzero decimal digit that
   does NOT divide num. */
static bool _a171492_includes(idx_t num)
{
    if (num == 0) return false;            /* zero has no nonzero digits */
    idx_t tmp = num;
    while (tmp > 0)
    {
        idx_t d = tmp % 10;
        if (d > 0 && (num % d) != 0)
            return true;
        tmp /= 10;
    }
    /* all nonzero digits divide num – it is excluded */
    return false;
}

/* helper to convert an idx_t to seq_t (works for builtin types and
   integer‑like big integer classes) */
static inline seq_t make_seq_t(idx_t x) { return (seq_t)x; }

#ifdef __cplusplus
/* cast a plain integer to the integer‑like sequence type when seq_t is a
   class; works for arbitrary‑precision builds */

#else
/* for builds where seq_t is a built‑in unsigned integer type */

#endif

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based; n == 0 is undefined – return 0. */
    if (n == 0) return (seq_t)0;

    idx_t count   = 0;
    idx_t current = 1;
    while (count < n)
    {
        if (_a171492_includes(current))
        {
            ++count;
            if (count == n)
                return make_seq_t(current);
        }
        ++current;
    }
    /* Unreachable – return 0 to satisfy the compiler. */
    return (seq_t)0;
}
