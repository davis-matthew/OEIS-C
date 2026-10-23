/*
 * A143580  Duplicate of A010059.
 * https://oeis.org/A143580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 - (popcount(n) mod 2) (complement of the Thue–Morse
 *            sequence)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long x = n;        /* copy index into machine word */
    unsigned int parity = 0;         /* parity of the popcount */

    /* toggle parity for each set bit: O(number of 1‑bits) */
    while (x) {
        parity ^= 1;
        x &= x - 1;                  /* clear lowest set bit */
    }

    /* return the complement of the parity: 1 if parity==0, else 0 */
    return (seq_t)(~parity & 1);
}
