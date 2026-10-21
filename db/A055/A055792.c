/*
 * A055792  a(n) and floor(a(n)/2) are both squares; i.e., squares which
 *          remain squares when written in base 2 and last digit is removed.
 * https://oeis.org/A055792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) for n > 3, with initial
 *            terms a(0)=0, a(1)=1, a(2)=9, a(3)=289
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)289;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)9;   /* a(n-1) */
    seq_t a3 = (seq_t)289; /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)35 * a3 - (seq_t)35 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
