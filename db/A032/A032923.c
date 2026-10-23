/*
 * A032923  Numbers whose set of base-16 digits is {1,3}.
 * https://oeis.org/A032923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with base-16 digits only 1 or 3 (sorted in increasing order) */

seq_t a(idx_t n)
{
    /* Determine the length L of the hexadecimal representation that
       contains the nth number.  The total count of all numbers with
       lengths up to L is sum_{k=1}^{L} 2^k = 2^{L+1} - 2. */
    idx_t L = 1;
    idx_t cum = 0;                 /* cumulative count up to current length */
    for (;; ++L) {
        cum += (idx_t)1 << L;      /* add 2^L numbers of length L */
        if (cum >= n) break;
    }

    /* Offset of n within the set of numbers of length L (0‑based). */
    idx_t prev_cum = cum - ((idx_t)1 << L);
    idx_t offset   = n - prev_cum - 1;

    /* Convert the offset into the hexadecimal number using digits
       1 (for bit 0) and 3 (for bit 1). */
    seq_t value = 0;
    for (idx_t i = L; i > 0; --i) {
        seq_t bit   = (offset >> (i - 1)) & 1;
        seq_t digit = bit ? (seq_t)3 : (seq_t)1;
        value = value * (seq_t)16 + digit;
    }
    return value;
}
