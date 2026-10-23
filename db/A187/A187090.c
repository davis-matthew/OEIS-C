/*
 * A187090  Smallest multiple of n beginning with 9.
 * https://oeis.org/A187090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the smallest multiple of n whose decimal representation
 *            * starts with the digit 9. The routine performs a brute‑force
 *            search * over successive multiples k·n until the leading digit
 *            of the product * equals 9. The product is computed using the
 *            arithmetic operators * defined for the build‑sp...
 * Range:     u8 n <= 16, u16 n <= 1111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid any truncation. */
    const seq_t num = (seq_t)n;

    /* Search over positive integer multipliers.  The loop is guaranteed
     * to terminate because among the infinite sequence of multiples
     * eventually one must have a leading digit 9. */
    for (idx_t k = 1;; ++k)
    {
        /* Compute k·n in the type seq_t.  The cast of k ensures that
         * the multiplication is performed in seq_t, avoiding
         * intermediate overflow for the integer types and delegating
         * to the arbitrary‑precision implementation for the big type. */
        seq_t prod = (seq_t)k * num;

        /* Extract the most significant decimal digit. */
        seq_t d = prod;
        while (d >= 10)
            d /= 10;

        if (d == 9)
            return prod;  /* smallest multiple starting with 9 found */
    }

    /* Unreachable, but keeps the compiler happy. */
    return (seq_t)0;
}
