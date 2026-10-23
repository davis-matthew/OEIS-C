/*
 * A135126  Numbers such that the digital sums in bases 3, 4, 5 and 6 all are
 *          equal.
 * https://oeis.org/A135126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n-th integer whose digit sums in bases 3, 4, 5 and 6 are equal. */
/* (No closed‑form formula is known; we generate by exhaustive search.) */

#include <stdint.h>
#include <stdbool.h>

/* Sum of digits of x in the given base.  */
static inline idx_t digit_sum(idx_t x, idx_t base)
{
    idx_t s = 0;
    while (x != 0)
    {
        s += x % base;
        x /= base;
    }
    return s;
}

/* Return true iff the four digit sums are all equal. */
static inline bool equal_base_sums(idx_t x)
{
    idx_t s3 = digit_sum(x, 3);
    idx_t s4 = digit_sum(x, 4);
    if (s3 != s4) return false;
    idx_t s5 = digit_sum(x, 5);
    if (s3 != s5) return false;
    idx_t s6 = digit_sum(x, 6);
    return s3 == s6;
}

/* Return the n-th term of the sequence (n >= 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety, offset is 1 */

    idx_t count = 0;
    idx_t x = 1;

    for (;; ++x)
    {
        if (equal_base_sums(x))
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
