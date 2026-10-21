/*
 * A089309  Write n in binary; a(n) = length of the rightmost run of 1's.
 * https://oeis.org/A089309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the rightmost run of 1's in the binary
 *            representation of n (OEIS A089309). Algorithm: * If n = 0,
 *            return 0. * Remove trailing zeros of n: n >>= tz0 (tz0 = # of
 *            trailing zeros). * The remaining number m is odd. Compute m+1
 *            and count its * trailing zeros; that count is a(n). This w...
 * Range:     u8 n <= 8811, u16 n <= 8811, u32 n <= 8811, u64 n <= 8811,
 *            u128 n <= 8811, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 1. Count trailing zeros of n */
    idx_t tz0 = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        ++tz0;
        tmp >>= 1;
    }

    /* 2. tmp is now the odd part m of n (n >> tz0) */
    idx_t m = tmp;

    /* 3. Compute m + 1 and count its trailing zeros */
    idx_t val = m + 1;
    idx_t tz1 = 0;
    while ((val & 1) == 0) {
        ++tz1;
        val >>= 1;
    }

    return (seq_t)tz1;
}
