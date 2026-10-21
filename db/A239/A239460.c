/*
 * A239460  Concatenation of n^2 and n^3.
 * https://oeis.org/A239460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n^2, n^3) in decimal representation
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int seq_digit_len(seq_t x) {
    int cnt = 0;
    while (x > 0) {
        x /= 10;
        cnt++;
    }
    return cnt;
}

static seq_t seq_pow10(int e) {
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;
    seq_t ns3 = ns2 * ns;
    int len = seq_digit_len(ns3);
    seq_t pow10len = seq_pow10(len);
    return ns2 * pow10len + ns3;
}
