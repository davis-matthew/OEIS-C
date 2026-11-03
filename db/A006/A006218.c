/*
 * A006218  a(n) = Sum_{k=1..n} floor(n/k); also Sum_{k=1..n} d(k), where d =
 *          number of divisors (A000005); also number of solutions to x*y = z
 *          with 1 <= x,y,z <= n.
 * https://oeis.org/A006218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(Sum_{i=1..floor(sqrt(n))} floor(n/i)) -
 *            floor(sqrt(n))^2
 * Range:     u8 n <= 59, u16 n <= 7247, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 76 terms through n = 9774 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t isqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    uint64_t sqrtn = isqrt((uint64_t)n);
    seq_t result = 0;
    
    for (uint64_t i = 1; i <= sqrtn; i++) {
        result += (seq_t)n / (seq_t)i;
    }
    
    result = result * 2 - (seq_t)sqrtn * (seq_t)sqrtn;
    
    return result;
}
