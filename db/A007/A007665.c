/*
 * A007665  Tower of Hanoi with 5 pegs.
 * https://oeis.org/A007665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + 2^j for C(j+2,3) < n-1 <= C(j+3,3)
 *            (the difference 2^j repeats C(j+2,2) times, j >= 1);
 *            equivalently a(n) = A055580(k) + (n - binomial(k+3,3))*2^k with
 *            k = A056556(n-1)
 * Range:     u8 n <= 29, u16 n <= 238, u32 n <= 2558, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    seq_t p = 1;
    idx_t remaining = n - 1;
    idx_t j = 1;
    while (remaining > 0) {
        idx_t c = (j + 1) * (j + 2) / 2;
        idx_t t = c < remaining ? c : remaining;
        p = p * 2;
        v += (seq_t)t * p;
        remaining -= t;
        j++;
    }
    return v;
}
