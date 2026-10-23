/*
 * A167778  Subsequence of A167708 whose indices are 2 mod 5.
 * https://oeis.org/A167778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 340*a(n+1) - a(n), a(0)=66, a(1)=22335
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)66;
    if (n == 1) return (seq_t)22335;
    seq_t a0 = (seq_t)66;          /* a(n-2) */
    seq_t a1 = (seq_t)22335;       /* a(n-1) */
    seq_t an = 0;                  /* a(n) to compute */
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)340 * a1 - a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
