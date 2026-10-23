/*
 * A147628  Weight distribution of [64,10,28] extended binary primitive BCH
 *          (or XBCH) code.
 * https://oeis.org/A147628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 for n = 0 or 16; 448 for n = 7 or 9; 126 for n = 8; 0
 *            otherwise
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight distribution of the [64,10,28] extended binary primitive BCH
   (or XBCH) code.  The only non‑zero terms are at the indices listed
   above.  All other indices give zero. */

seq_t a(idx_t n)
{
    if (n == 0 || n == 16) return (seq_t)1;
    if (n == 7 || n == 9)  return (seq_t)448;
    if (n == 8)             return (seq_t)126;
    return (seq_t)0;
}
