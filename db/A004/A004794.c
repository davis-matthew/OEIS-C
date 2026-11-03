/*
 * A004794  Least positive integer k such that the fractional part of
 *          k*sqrt(5) has its n initial partial quotients all equal to 1.
 * https://oeis.org/A004794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fib(12*floor(n/6) + S_(n mod 6)) + 1)/2, S = (2, 5, 7,
 *            7, 10, 11); computed as Fib(k-2) + ceil(Fib(k-3)/2), since
 *            Fib(k) = 2*Fib(k-2) + Fib(k-3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling: sets *f0 = F(m), *f1 = F(m+1). Unsigned wraparound is
   harmless: F(k-2) and F(k-3) both fit whenever a(n) itself does. */
static void fib_pair(uint64_t m, seq_t *f0, seq_t *f1) {
    seq_t x = 0, y = 1; /* F(0), F(1) */
    int bit = 63;
    while (bit >= 0 && !((m >> bit) & 1)) bit--;
    for (; bit >= 0; bit--) {
        seq_t c = x * (y + y - x);   /* F(2j) */
        seq_t d = x * x + y * y;     /* F(2j+1) */
        if ((m >> bit) & 1) {
            x = d;
            y = c + d;
        } else {
            x = c;
            y = d;
        }
    }
    *f0 = x;
    *f1 = y;
}

seq_t a(idx_t n) {
    static const int S[] = {2, 5, 7, 7, 10, 11};
    uint64_t k = 12 * (uint64_t)(n / 6) + (uint64_t)S[n % 6];
    seq_t f3, f2;
    /* k >= 5 for n >= 1 */
    fib_pair(k - 3, &f3, &f2);       /* f3 = F(k-3), f2 = F(k-2) */
    return f2 + f3 / 2 + (f3 & 1);
}
