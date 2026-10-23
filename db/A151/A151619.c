/*
 * A151619  Weight distribution of [256,29,96] extended binary primitive BCH
 *          (or XBCH) code.
 * https://oeis.org/A151619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the number of codewords of weight 8*n in the extended
 *            binary primitive BCH [256,29,96] code. All other terms are zero.
 *            The non‑zero values are: a(0) = 1 a(12) = 359856 a(14) =
 *            37361920 a(15) = 113229824 a(16) = 234967710 a(17) = 113229824
 *            a(18) = 37361920 a(20) = 359856 a(32) = 1 For any...
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
    case 0:  return (seq_t)1;
    case 12: return (seq_t)359856;
    case 14: return (seq_t)37361920;
    case 15: return (seq_t)113229824;
    case 16: return (seq_t)234967710;
    case 17: return (seq_t)113229824;
    case 18: return (seq_t)37361920;
    case 20: return (seq_t)359856;
    case 32: return (seq_t)1;
    default: return (seq_t)0;
    }
}
