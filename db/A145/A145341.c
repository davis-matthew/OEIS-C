/*
 * A145341  Convert 2n-1 to binary. Reverse its digits. Convert back to
 *          decimal to get a(n).
 * https://oeis.org/A145341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(binary(2*n-1)) (OEIS A145341)
 * Range:     u8 n <= 128, u16 n <= 9539, u32 n <= 9539, u64 n <= 9539,
 *            u128 n <= 9539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n-1 */
    seq_t val = (seq_t)n;
    val = val * 2 - 1;

    /* Copy the value to work on its bits */
    seq_t orig = val;
    seq_t rev  = 0;

    /* Determine the number of significant bits in val */
    idx_t len = 0;
    seq_t tmp = orig;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Reverse the bits */
    for (idx_t i = 0; i < len; ++i) {
        rev = (rev << 1) | (orig & 1);
        orig >>= 1;
    }

    return rev;
}
