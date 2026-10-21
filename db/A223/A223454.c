/*
 * A223454  Number of idempotent 2 X 2 -n..n matrices.
 * https://oeis.org/A223454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 // base idempotents with b=c=0 + 8*n // cases (b=0,c≠0)
 *            and (c=0,b≠0) + Σ_{a=-n..n, a≠0,1} f(a,n) where f(a,n) counts
 *            the ordered pairs (b,c) (both non‑zero, |b|,|c|≤n) satisfying a²
 *            - a + b·c = 0, i.e. b·c = a(1-a)
 * Range:     u8 n <= 13, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Count the number of idempotent 2×2 matrices with entries in [-n..n] */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t result = 4 + (seq_t)8 * (seq_t)n;   /* base + 8n cases */

    /* Loop over integer a in [-n .. n], excluding 0 and 1 */
    for (long long a = -(long long)n; a <= (long long)n; ++a) {
        if (a == 0 || a == 1) continue;

        long long val = a * (1 - a);          /* a*(1-a) */
        if (val == 0) continue;              /* should not occur */

        long long absval = val > 0 ? val : -val;
        /* integer sqrt of absval */
        long long limit = (long long)sqrt((long double)absval);
        if ((limit + 1) * (limit + 1) <= absval) ++limit;
        while (limit * limit > absval) --limit;

        for (long long d = 1; d <= limit; ++d) {
            if (absval % d != 0) continue;
            long long q = absval / d;

            /* Check bounds for b and c */
            if (d > (long long)n || q > (long long)n) continue;

            if (d == q) {
                /* d == q : only two ordered pairs */
                result += (seq_t)2;
            } else {
                /* d != q : four ordered pairs */
                result += (seq_t)4;
            }
        }
    }

    return result;
}
