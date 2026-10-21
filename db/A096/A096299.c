/*
 * A096299  List of strings in lexicographic order with property that for the
 *          2^(m-1) strings of length m, the first entry is 1, the second
 *          distinct entry (reading from left to right) is 2, the third
 *          distinct entry is 3, etc.
 * https://oeis.org/A096299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1022, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence  A096299 : strings in lexicographic order where each new
   distinct digit equals the next integer value.  The implementation
   converts the index n into a binary pattern that determines when a new
   digit is introduced.  It works for any seq_t and idx_t supplied by
   the build environment. */

seq_t a(idx_t n)
{
    /* Find the smallest m such that 2^m >= n+1.
       That gives the block of strings of length m. */
    idx_t m   = 0;
    idx_t pow = 1;
    while (pow < n + 1) {
        pow <<= 1;
        ++m;
    }

    /* Offset within the block of strings of length m (0‑based). */
    idx_t offset = n - (pow >> 1);    /* pow is 2^m, so pow>>1 = 2^(m‑1) */

    /* Build the decimal number digit by digit. */
    seq_t result = (seq_t)1;          /* first digit is always 1 */
    seq_t cur    = 1;                 /* current distinct digit value */

    /* For each of the remaining m‑1 positions we decide whether to
       introduce a new digit (shift by the corresponding bit of offset). */
    idx_t bits = m - 1;
    for (idx_t i = 0; i < bits; ++i) {
        /* Check the bit (m‑2‑i) of offset. */
        if (offset & ((idx_t)1 << (bits - 1 - i))) {
            ++cur;
        }

        /* Append cur to the decimal representation of result. */
        seq_t pow10 = (cur < 10 ? 10 : 100);   /* cur <= 64 for the problem constraints */
        result = result * pow10 + cur;
    }

    return result;
}
