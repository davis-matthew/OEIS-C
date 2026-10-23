/*
 * A120664  Expansion of 2*x*(1-6*x+12*x^2)/(1-8*x+19*x^2-12*x^3).
 * https://oeis.org/A120664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120664:  a(0)=0, a(1)=2, a(2)=4, a(3)=18;
   for n>3  a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3) */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)18;

    /* initialise with the values for indices 1, 2, 3 */
    seq_t a_nm3 = (seq_t)2; /* a(n-3) when n=4 */
    seq_t a_nm2 = (seq_t)4; /* a(n-2) when n=4 */
    seq_t a_nm1 = (seq_t)18;/* a(n-1) when n=4 */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_n = 8 * a_nm1 - 19 * a_nm2 + 12 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
