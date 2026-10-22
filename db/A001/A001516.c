/*
 * A001516  Bessel polynomial {y_n}''(1).
 * https://oeis.org/A001516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} k*(k-1)*binomial(n+k,2k)*(2k-1)!! (y_n''(1),
 *            from y_n(x) = Sum_k (n+k)!/((n-k)!k!)*(x/2)^k); for arbitrary
 *            precision the equivalent D-finite recurrence (n-2)*(n-1)*a(n) =
 *            (2n-1)*(n^2-n+2)*a(n-1) + n*(n+1)*a(n-2), a(1) = 0, a(2) = 6, is
 *            used
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   a001516_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001516_is_fixed.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;

    if (a001516_is_fixed()) {
        /* Every term is computed with only + and *, so it is exact modulo 2^w.
           binomial(n+k,2k) stays small (it is <= a(n)) for any n where a(n) fits. */
        seq_t s = 0;
        seq_t df = 1;                 /* (2k-1)!! */
        unsigned __int128 C = 1;      /* binomial(n+k, 2k) */
        for (idx_t k = 0; k <= n; k++) {
            if (k >= 2)
                s += (seq_t)k * (seq_t)(k - 1) * (seq_t)(uint64_t)C * df;
            df *= (seq_t)(2 * k + 1);
            if (k < n)
                C = C * (unsigned __int128)(n + k + 1) * (unsigned __int128)(n - k)
                    / ((unsigned __int128)(2 * k + 1) * (unsigned __int128)(2 * k + 2));
        }
        return s;
    }

    if (n == 2) return 6;
    seq_t p2 = 0;  /* a(i-2) */
    seq_t p1 = 6;  /* a(i-1) */
    for (idx_t i = 3; i <= n; i++) {
        seq_t x = (seq_t)i;
        seq_t num = (2 * x - 1) * (x * x - x + 2) * p1 + x * (x + 1) * p2;
        seq_t den = (x - 2) * (x - 1);
        seq_t cur = num / den;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
