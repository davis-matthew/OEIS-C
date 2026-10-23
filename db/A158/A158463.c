/*
 * A158463  a(n) = 12*n^2 - 1.
 * https://oeis.org/A158463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12*n^2 - 1
 * Range:     i8 n <= 3, i16 n <= 52, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to keep the
       arithmetic in the appropriate integer range. */
    seq_t sn = (seq_t)n;

    /* Compute 12 * n^2 - 1.  The intermediate value n*n fits
       within seq_t for all n used in the tests (up to ~10 000
       for the 64‑bit build).  Multiplying by 12 then subtracting 1
       gives the desired result. */
    seq_t val = sn * sn;   /* n² */
    val *= 12;             /* 12*n² */
    val -= 1;              /* 12*n² - 1 */

    return val;
}
