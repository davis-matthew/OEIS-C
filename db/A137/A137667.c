/*
 * A137667  The sequence alternates even and odd integers but also even and
 *          odd digits; it is monotonically increasing and a(n) is always the
 *          smallest integer fitting the pattern.
 * https://oeis.org/A137667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A137667.
   For n < 10, a(n) = n. For n ≥ 10, a(n) is constructed as the
   smallest integer larger than a(n−1) that has:
     • parity of the integer equal to n mod 2
     • leading digit equal to the same parity
     • each pair of adjacent digits has opposite parity
   The construction can be done deterministically by building the
   digits from most significant to least significant.  The leading
   digit is 2 + m, where m = (n − 10) mod 8.  The remaining digits are
   chosen to be 0 for even parity and 1 for odd parity, alternating
   starting with the parity opposite to that of the leading digit. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* 0‑9 are just the digits themselves. */
    if (n < 10)
        return (seq_t)n;

    /* Compute group index and offset within the group. */
    idx_t offset = n - 10;
    idx_t k = offset / 8;          /* group number (0‑based)   */
    idx_t m = offset % 8;          /* position within group    */

    /* Leading digit for this term. */
    seq_t d = (seq_t)(2 + m);       /* 2‑9 */
    seq_t res = d;

    /* Total number of digits: start with 3, add 2 for each group. */
    size_t len = (size_t)(3 + 2 * k);

    /* Parity of the leading digit (0 = even, 1 = odd). */
    int d_int = (int)d;             /* safe: d is ≤ 9 */
    int p = d_int & 1;              /* parity of leading digit */

    /* Build the remaining digits from most significant to least. */
    for (size_t pos = 2; pos <= len; ++pos) {
        int parity = (pos & 1) ? p : 1 - p;   /* alternate parity */
        seq_t digit = parity ? (seq_t)1 : (seq_t)0;
        res = res * 10 + digit;
    }

    return res;
}
