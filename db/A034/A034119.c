/*
 * A034119  Fractional part of cube root of a(n) starts with digit 3.
 * https://oeis.org/A034119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A034119: integers a(n) such that the fractional part of the cube
 * root of a(n) begins with digit 3.  For an integer m,
 * a lies in the interval [ceil((m+0.3)^3), floor((m+0.4)^3) - 1].
 * The implementation iterates over m, computes the lower and upper bounds
 * of this range using integer arithmetic, and returns the n-th smallest value.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;              /* number of terms already accounted for */

    for (idx_t m = 1; ; ++m) {    /* iterate over possible integer parts of the cube root */
        seq_t tm   = (seq_t)m;    /* cast m to seq_t to avoid overflow of 10*m */
        seq_t b1   = 10 * tm + 3; /* 10*m + 3  (for (m + 0.3)^3)   */
        seq_t low  = (b1 * b1 * b1 + 999) / 1000;   /* ceil((m+0.3)^3) */

        seq_t b2   = 10 * tm + 4; /* 10*m + 4  (for (m + 0.4)^3)   */
        seq_t high = (b2 * b2 * b2 - 1) / 1000;     /* floor((m+0.4)^3) - 1 */

        if (low > high) continue; /* no terms for this m */

        idx_t width = (idx_t)(high - low + 1); /* number of terms in this block */

        if (n <= count + width) {
            /* the desired term lies within this block */
            seq_t offset = (seq_t)(n - count - 1);
            return low + offset;
        }

        count += width;           /* skip this whole block */
    }
}
