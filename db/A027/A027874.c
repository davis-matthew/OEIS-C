/*
 * A027874  Minimal degree path length of a tree with n leaves.
 * https://oeis.org/A027874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*q*n+2*(n-3^q) for 2*3^(q-1)<=n<=3^q * a(n) =
 *            3*q*n+4*(n-3^q) for 3^q<=n<=2*3^q * where q is the smallest
 *            integer with 2*3^(q-1) <= n <= 2*3^q
 * Range:     u8 n <= 27, u16 n <= 2971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t q = 0;
    seq_t p = 1;             /* p = 3^q */
    while ((seq_t)n > p * (seq_t)2) {  /* find smallest q such that 2*p >= n */
        p *= 3;
        q++;
    }
    seq_t term = (seq_t)3 * (seq_t)q * (seq_t)n;
    if ((seq_t)n <= p) {          /* first range */
        seq_t diff = p - (seq_t)n;       /* p >= n */
        return term - (seq_t)2 * diff;
    } else {                      /* second range */
        seq_t diff = (seq_t)n - p;       /* n > p */
        return term + (seq_t)4 * diff;
    }
}
