/*
 * A001273  Smallest number that takes n steps to reach 1 under iteration of
 *          sum-of-squares-of-digits map (= smallest "happy number" of height
 *          n).
 * https://oeis.org/A001273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) for n <= 7 by direct search (smallest k taking exactly n
 *            steps of the sum-of-squares-of-digits map to reach 1); for n >=
 *            7, a(n) = k(n)*10^((a(n-1)-A003132(k(n)-1))/81) - 1, with k(8) =
 *            3789. a(n) for n >= 9 has about 10^973 digits or more, so it
 *            cannot be represented
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t a001273_ssd(uint64_t x) {
    uint64_t s = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        s += d * d;
        x /= 10;
    }
    return s;
}

static int a001273_height(uint64_t x) {
    int c = 0;
    while (x != 1) {
        x = a001273_ssd(x);
        c++;
        if (c > 64) return -1;
    }
    return c;
}

static uint64_t a001273_small(idx_t n) {
    uint64_t m;
    for (m = 1; m < 10000000ULL; m++)
        if (a001273_height(m) == (int)n) return m;
    return 0;
}

seq_t a(idx_t n) {
    static const uint64_t kv[] = {79, 3789, 78889, 259, 179, 47, 137, 1128,
                                  58, 228, 19, 34, 145};
    if (n <= 7) return (seq_t)a001273_small(n);
    if (n == 8) {
        uint64_t prev = a001273_small(7);
        uint64_t k = kv[n - 7];
        uint64_t e = (prev - a001273_ssd(k - 1)) / 81;
        seq_t r = (seq_t)k;
        uint64_t i;
        for (i = 0; i < e; i++) r *= 10;
        return r - 1;
    }
    return 0;
}
