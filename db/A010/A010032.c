/*
 * A010032  Weight distribution of binary (16,256,6) nonlinear
 *          Nordstrom-Robinson code.
 * https://oeis.org/A010032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 if n=0, 112 if n=3, 30 if n=4, 112 if n=5, 1 if n=8,
 *            otherwise 0; a(n)=a(16−n) for 0≤n≤16, and a(n)=0 for n>16
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 16) return (seq_t)0;
    /* Use symmetry about 8: a(n)=a(16−n). */
    idx_t k = (n < 8) ? n : 16 - n;
    switch (k) {
        case 0: return (seq_t)1;
        case 3: return (seq_t)112;
        case 4: return (seq_t)30;
        case 5: return (seq_t)112;
        case 8: return (seq_t)1;
        default: return (seq_t)0;
    }
}
