/*
 * A133681  a(n)=9*a(n-1)+90*a(n-2) for n>=3, a(0)=1, a(1)=9, a(2)=162 .
 * https://oeis.org/A133681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1)+90*a(n-2) for n>=3,
   a(0)=1, a(1)=9, a(2)=162 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)162;
    seq_t a1 = (seq_t)9;   /* a(n-1)   */
    seq_t a2 = (seq_t)162; /* a(n)     */
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)9 * a2 + (seq_t)90 * a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
