/*
 * A127367  Inverse permutation to A127366.
 * https://oeis.org/A127367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let m be the largest integer such that m(m+1) ≤ n. Then a(n) = n
 *            - m if n is even, otherwise a(n) = n + m + 1
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t find_m(idx_t n) {
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);          /* safe upper bound for m */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t prod = (__uint128_t)mid * (mid + 1);
        if (prod <= n)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;                            /* largest m with m(m+1) ≤ n */
}

seq_t a(idx_t n) {
    idx_t m = find_m(n);
    if ((n & 1ULL) == 0)   /* n even */
        return (seq_t)n - (seq_t)m;
    else                  /* n odd */
        return (seq_t)n + (seq_t)m + 1;
}
