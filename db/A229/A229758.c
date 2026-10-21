/*
 * A229758  Triangle T(n,k) = A007953(n) * A007953(k) - A007953(n*k) with 1 <=
 *          k <= n.
 * https://oeis.org/A229758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = A007953(m) * A007953(k) - A007953(m*k) where m =
 *            floor((sqrt(1+8n)-1)/2) and k = n - m(m-1)/2
 * Range:     u8 n <= 724, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(seq_t x) {
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    /* Find the row m such that m(m+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1; /* double until an upper bound is found */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Determine position k within the row */
    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev;

    /* Compute the required digit sums */
    seq_t sm  = digit_sum(m);
    seq_t sk  = digit_sum(k);
    seq_t smk = digit_sum(m * k);

    return sm * sk - smk;
}
