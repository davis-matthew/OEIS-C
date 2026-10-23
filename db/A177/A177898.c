/*
 * A177898  Duplicate of A168198.
 * https://oeis.org/A177898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*n/2 + 1 if n is even * a(n) = (3*n+9)/2 if n is odd
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177898 */

seq_t a(idx_t n)
{
    /* half = floor(n/2), calculated in seq_t to avoid overflow for the
       intermediate addition. */
    seq_t half = (seq_t)(n >> 1);

    if ((n & 1ULL) == 0)            /* even n */
        return 3 * half + 1;        /* 3*n/2 + 1 */
    else                            /* odd n */
        return 3 * half + 6;        /* (3*n+9)/2 */
}
