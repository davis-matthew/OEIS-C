/*
 * A033768  Product t2(q^d); d | 9, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions of a^2 + 3 b^2 + 9 c^2 =
 *            8 n + 13. Equivalent to counting non‑negative triples (A,B,C)
 *            with A(A+1)+3 B(B+1)+9 C(C+1) = 2 n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_sqrt_u64(idx_t x) {
    idx_t low = 0, high = (idx_t)1ULL << 32;          /* high = 2^32  > sqrt(2^64)   */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t two_n = (idx_t)2 * n;                     /* 2 * n  (fits in idx_t for n <= 1e4) */
    for (idx_t C = 0; ; ++C) {
        idx_t TC = C * (C + 1);                      /* C(C+1) */
        idx_t subtotalC = (idx_t)9 * TC;             /* 9 * TC */
        if (subtotalC > two_n) break;
        for (idx_t B = 0; ; ++B) {
            idx_t TB = B * (B + 1);                  /* B(B+1) */
            idx_t subtotal = subtotalC + (idx_t)3 * TB;  /* subtotalC + 3*TB */
            if (subtotal > two_n) break;
            idx_t rem = two_n - subtotal;            /* rem = 2n - subtotal  */
            idx_t D = (idx_t)1 + (idx_t)4 * rem;     /* 1 + 4 * rem          */
            idx_t s = floor_sqrt_u64(D);             /* integer sqrt of D    */
            if ((unsigned __int128)s * s == D && (s & 1)) {
                ++count;                             /* unique A for this B,C */
            }
        }
    }
    return count;
}
