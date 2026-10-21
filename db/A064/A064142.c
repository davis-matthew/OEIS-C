/*
 * A064142  Sum of all distinct primes dividing central binomial coefficient
 *          C(n, floor(n/2)).
 * https://oeis.org/A064142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct primes dividing the central binomial
 *            coefficient C(n, ⌊n/2⌋)
 * Range:     u8 n <= 50, u16 n <= 1032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Computes a(n) for n >= 1 as defined above.
   Complexity: O(n log log n) due to the sieve and a small factor loop for each prime. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;               /* Undefined for n=0, return 0 as safe default */

    /* Pre‑compute the primes up to n using a simple sieve on each call.
       For n <= 10000 this is trivial and fast enough even for many calls. */
    idx_t m = n >> 1;            /* floor(n/2) */
    seq_t sum = 0;

    /* Maximum n that we will handle – fits in int for n <= 10000 */
    int max_n = (int)n;

    /* Sieve up to max_n */
    bool is_comp[max_n + 1];
    for (int i = 0; i <= max_n; ++i)
        is_comp[i] = false;
    for (int i = 2; i * i <= max_n; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= max_n; j += i)
                is_comp[j] = true;
        }
    }

    /* For each prime compute its exponent in C(n, m) via Legendre's formula,
       and include it in the sum if the exponent is positive. */
    for (int p = 2; p <= max_n; ++p) {
        if (is_comp[p])
            continue;              /* Skip composites */

        idx_t e = 0;
        idx_t pw = (idx_t)p;
        while (pw <= n) {
            e += n / pw - m / pw - (n - m) / pw;
            if (pw > n / p)
                break;            /* Prevent overflow of pw *= p */
            pw *= p;
        }
        if (e > 0)
            sum += (seq_t)p;       /* Add distinct prime to the sum */
    }

    return sum;
}
