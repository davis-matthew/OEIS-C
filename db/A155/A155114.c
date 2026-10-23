/*
 * A155114  Number of ways to express n as the sum of an odd prime, a positive
 *          Fibonacci number and twice a positive Fibonacci number.
 * https://oeis.org/A155114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = #{ <p,s,t> : p + F_s + 2*F_t = n, * p odd prime, s > 1, t
 *            > 1 }
 * Range:     u8 n <= 966, u16 n <= 8769, u32 n <= 8769, u64 n <= 8769,
 *            u128 n <= 8769, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/is_prime.h"

/* Compute a(n) for any n >= 1.
 * Uses a simple O(k^2) search where k is the number of Fibonacci
 * numbers <= n.  k is <= 50 for n <= 10⁴ with 64‑bit integer types,
 * easily below the timing limit. */
seq_t a(idx_t n)
{
    /* Fibonacci values F_1 = 1, F_2 = 1, ...  */
    seq_t fibs[64];
    size_t fib_cnt = 0;

    fibs[0] = 1;              /* F_1 */
    fibs[1] = 1;              /* F_2 */
    fib_cnt = 2;

    /* Extend fibs until the last value exceeds n.  */
    seq_t n_seq = (seq_t)n;
    while (fibs[fib_cnt - 1] <= n_seq) {
        fibs[fib_cnt] = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        fib_cnt++;
    }

    seq_t count = 0;

    /* s > 1   → index 1 and above in fibs array. */
    for (size_t si = 1; si < fib_cnt; ++si) {
        seq_t Fs = fibs[si];
        if (Fs > n_seq) break;         /* further s only increase Fs */

        /* t > 1   → index 1 and above.          */
        for (size_t ti = 1; ti < fib_cnt; ++ti) {
            seq_t Ft = fibs[ti];
            seq_t twoFt = Ft + Ft;    /* 2 * Ft */

            seq_t sum = Fs + twoFt;
            if (sum > n_seq) break;  /* further t only increase sum */

            idx_t p = n - (idx_t)sum;  /* candidate prime */
            if (p > 0 && (p & 1U) && is_prime(p)) /* odd prime check */
                count++;
        }
    }

    return count;
}
