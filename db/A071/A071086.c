/*
 * A071086  Number of integers of the form (nk)^2/(n^2+k^2), k>=1.
 * https://oeis.org/A071086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of k≥1 such that (n*k)^2/(n^2+k^2) is an integer
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count admissible k for a given divisor g of n */
static inline seq_t count_for_g(idx_t n, idx_t g)
{
    idx_t a = n / g;               /* a = n/g   (coprime factor) */
    if (a * a >= g * g)            /* no solutions if a^2 ≥ g^2 */
        return 0;

    idx_t rem = g * g - a * a;     /* maximum allowed b^2 */
    seq_t cnt = 0;
    for (idx_t b = 1; b * b <= rem; ++b) {
        if (gcd_idx(a, b) != 1)    /* need gcd(a,b)=1 */
            continue;
        idx_t sum = a * a + b * b; /* a^2 + b^2 */
        if ((g * g) % sum == 0)    /* g^2 divisible by a^2+b^2 */
            ++cnt;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;
        idx_t g1 = d;
        idx_t g2 = n / d;
        total += count_for_g(n, g1);
        if (g1 != g2)
            total += count_for_g(n, g2);
    }
    return total;
}
