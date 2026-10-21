/*
 * A090937  a(1) = 1, a(2) = 2; for n > 2, a(n) = a(n-1) + (smallest integer
 *          >= n which is coprime to a(n-1)).
 * https://oeis.org/A090937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=2, for n>2 a(n)=a(n-1)+ smallest integer >= n that
 *            is coprime to a(n-1). For n≥8 the closed form is
 *            a(n)=ceil((n+1)²/2)
 * Range:     u8 n <= 14, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* compute directly for small n (3…7) to avoid a table */
    if (n < 8) {
        seq_t prev = (seq_t)2;
        for (idx_t k = 3; k <= n; ++k) {
            seq_t i = (seq_t)k;
            while (seq_gcd(prev, i) != 1) ++i;
            prev += i;
        }
        return prev;
    }
    /* use the closed form for n ≥ 8 */
    seq_t val = (seq_t)(n + 1);
    val = val * val;        /* (n+1)²  */
    return (val + 1) / 2;   /* ceil division by 2 */
}
