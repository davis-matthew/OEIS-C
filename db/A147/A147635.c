/*
 * A147635  Weight distribution of [128,22,48] extended binary primitive BCH
 *          (or XBCH) code.
 * https://oeis.org/A147635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight distribution of the [128,22,48] extended binary primitive BCH code.
   a(n) = number of codewords with Hamming weight 8*n.
   Non‑zero coefficients occur only for the following indices:
        n = 0   ->  1
        n = 6   -> 42672
        n = 7   -> 877824
        n = 8   -> 2353310
        n = 9   -> 877824
        n =10   -> 42672
        n =16   ->  1
   All other a(n) equal 0.  The function returns 0 for any n > 16. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 6) return 42672;
    if (n == 7) return 877824;
    if (n == 8) return 2353310;
    if (n == 9) return 877824;
    if (n == 10) return 42672;
    if (n == 16) return 1;
    return 0;
}
