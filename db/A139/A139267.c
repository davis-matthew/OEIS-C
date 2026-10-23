/*
 * A139267  Twice octagonal numbers: 2*n*(3*n-2).
 * https://oeis.org/A139267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(3*n - 2)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of twice octagonal numbers.  Computes a(n) for any n (n >= 0). */
seq_t a(idx_t n)
{
    /* Convert the index to the proper sequence type first. */
    seq_t nn = (seq_t)n;
    /* Compute the formula in this type to avoid any truncation or overflow
       for the supported sequence types (unsigned integers up to 128 bits or
       arbitrary‑precision). */
    return 2 * nn * (3 * nn - 2);
}
