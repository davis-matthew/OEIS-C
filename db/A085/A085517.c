/*
 * A085517  Weight enumerator of [16,7,6] extended BCH code.
 * https://oeis.org/A085517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   For the extended binary [16,7,6] BCH code, a(n) equals the
 *            number of codewords whose Hamming weight is 2*n. The possible
 *            non‑zero weights are 0,6,8,10 and 16, with multiplicities
 *            1,48,30,48 and 1 respectively. For all other n the value is 0
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* weight w = 2*n (even weights only) */
    seq_t w = (seq_t)n * 2;

    /* weights larger than the code length give no codewords */
    if (w > 16) return (seq_t)0;

    /* return the appropriate multiplicity */
    switch ((int)w) {
        case 0:  return (seq_t)1;   /* weight 0   */
        case 6:  return (seq_t)48;  /* weight 6   */
        case 8:  return (seq_t)30;  /* weight 8   */
        case 10: return (seq_t)48;  /* weight 10  */
        case 16: return (seq_t)1;   /* weight 16  */
        default: return (seq_t)0;   /* all other even weights have 0 codewords */
    }
}
