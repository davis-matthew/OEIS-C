/*
 * A004736  Triangle read by rows: row n lists the first n positive integers
 *          in decreasing order.
 * https://oeis.org/A004736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = (t*t+3*t+4)/2-n, where t = floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t isqrt(uint64_t x) {
    if (x == 0) return 0;
    uint64_t guess = x;
    uint64_t prev;
    while (1) {
        prev = guess;
        guess = (guess + x / guess) / 2;
        if (guess >= prev) break;
    }
    return guess;
}

seq_t a(idx_t n) {
    uint64_t sqrt_val = isqrt(8 * n - 7);
    uint64_t t = (sqrt_val - 1) / 2;
    uint64_t result = (t * t + 3 * t + 4) / 2 - n;
    return (seq_t)result;
}
