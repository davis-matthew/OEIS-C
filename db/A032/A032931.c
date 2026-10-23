/*
 * A032931  Numbers whose set of base-11 digits is {1,2}.
 * https://oeis.org/A032931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose base‑11 digits belong to {1,2}.  
 *  The enumeration is by increasing numeric value, which is the same as
 *  grouping the numbers by length (number of base‑11 digits) and then
 *  interpreting the offset within each group as a binary number whose
 *  bits 0/1 correspond to digits 1/2 respectively.  */
seq_t a(idx_t n)
{
    /* Find the minimal power of two 2^m ≥ n+2.  Then
     *  the length of the base‑11 representation is L = m-1.  */
    idx_t pow2 = 1;
    while (pow2 < n + 2)
        pow2 <<= 1;                /* pow2 = 2^m */

    idx_t L = 0;
    idx_t tmp = pow2 >> 1;        /* tmp = 2^(m-1) = 2^L */
    while (tmp > 1) {
        tmp >>= 1;
        L++;
    }

    /* Number of terms having fewer than L digits: total_prev = 2^L - 2 */
    idx_t total_prev = (pow2 >> 1) - 2;

    /* Offset within the block of length L (0‑based) */
    idx_t pos = n - total_prev - 1;

    /* Build the value by processing the bits of pos from most significant to least,
     * mapping bit 0 → digit 1, bit 1 → digit 2, and accumulating in base‑11. */
    seq_t val = 0;
    for (idx_t i = L; i > 0; --i) {
        int bit = (pos >> (i - 1)) & 1;
        val = val * 11 + (1 + bit);
    }
    return val;
}
