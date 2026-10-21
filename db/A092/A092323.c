/*
 * A092323  2^m - 1 appears 2^m times.
 * https://oeis.org/A092323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^floor(log2(n))) - 1, with a(1)=0, for n >= 1
 * Range:     u8 n <= 511, u16 n <= 7229, u32 n <= 7229, u64 n <= 7229,
 *            u128 n <= 7229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle possible out‑of‑range index (not part of the OEIS definition). */
    if (n == 0) return 0;

    /* Compute m = floor(log2(n)) by counting right shifts. */
    idx_t m = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++m;
    }

    /* Compute 2^m - 1 using a single shift and subtraction. */
    seq_t val = ((seq_t)1 << m) - 1;
    return val;
}
