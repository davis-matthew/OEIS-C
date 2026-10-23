/*
 * A165760  a(n) = (16-9*8^n)/7.
 * https://oeis.org/A165760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 20, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165760
   Recurrence: a(0) = 1,   a(n) = 8*a(n-1) - 16   for n >= 1
   Equivalently: a(n) = (16 - 9*8^n)/7
   The implementation uses the simple linear recurrence, which keeps
   intermediate values within the range of the next term.  This works
   for all valid n in the 64‑bit build (where the terms fit in
   int64_t) and for the arbitrary‑precision build (where the
   intermediate products are handled by the big‑integer type).  The
   algorithm runs in O(n) arithmetic operations and satisfies the
   performance requirement for n up to about 10 000. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* compute a(n) */
    {
        val = (seq_t)8 * val - (seq_t)16;
    }
    return val;
}
