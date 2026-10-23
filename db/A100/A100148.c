/*
 * A100148  Structured small rhombicosidodecahedral numbers.
 * https://oeis.org/A100148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (108*n^3 - 150*n^2 + 48*n)/6 = 18*n^3 - 25*n^2 + 8*n
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 620, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=60, a(3)=285, a(4)=784, ... (OEIS A100148) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, return 0 for safety */

    seq_t sn  = (seq_t)n;          /* n as seq_t */
    seq_t n2  = sn * sn;           /* n^2  */
    seq_t n3  = n2 * sn;           /* n^3  */

    seq_t term1 = n3 * ((seq_t)18);    /* 18*n^3  */
    seq_t term2 = n2 * ((seq_t)25);    /* 25*n^2  */
    seq_t term3 = sn * ((seq_t)8);     /*  8*n    */

    /* term1 + term3 > term2 for all n >= 1, so no unsigned underflow */
    return term1 - term2 + term3;
}
