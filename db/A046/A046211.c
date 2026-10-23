/*
 * A046211  Number of ternary Lyndon words whose digits sum to 1 (or 2) mod 3;
 *          number of trace 1 (or 2) monic irreducible polynomials over GF(3).
 * https://oeis.org/A046211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 1/(3*n) * Σ_{d|n , gcd(d,3)=1} μ(d) * 3^(n/d) * * The
 *            function implements this exactly, using an intermediate *
 *            128‑bit variable when seq_t is no larger than 64‑bit, and *
 *            otherwise using the arbitrary‑precision pow3 utility. * * The
 *            implementation works for any seq_t supplied by the bui...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   mu_of, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3.h"     /* big‑integer 3^exp (seq_t) */
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    /* Special case for the empty sum (should not happen, n >= 1). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Determine at compile time whether seq_t fits in 64 bits. */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 big_t;
#else
    /* Fallback: use seq_t itself; this may overflow if seq_t is 64‑bit
       but the code path below is only used when seq_t is 64‑bit or smaller. */
    typedef seq_t big_t;
#endif

    /* If seq_t is 64‑bit or smaller we use 128‑bit arithmetic for
       the intermediate sum.  Otherwise we use big‑integer arithmetic. */
    if (sizeof(seq_t) <= 8) {
        /* Pre‑compute powers of 3 up to n in 128‑bit. */
        big_t *pow3 = (big_t *)malloc((size_t)(n + 1) * sizeof(big_t));
        if (!pow3)
            return (seq_t)0;      /* allocation failure – unlikely in tests */

        pow3[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            pow3[i] = pow3[i - 1] * (big_t)3;

        big_t sum = 0;

        /* Enumerate divisors of n. */
        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i != 0)
                continue;

            /* divisor d = i */
            if (i % 3 != 0) {
                int m = mu_of(i);      /* μ(i) */
                if (m) {
                    idx_t exp = n / i;
                    big_t term = pow3[exp];
                    if (m > 0)
                        sum += term;
                    else
                        sum -= term;
                }
            }

            /* complementary divisor d = n/i, skip if equal to i. */
            idx_t j = n / i;
            if (j != i && j % 3 != 0) {
                int m = mu_of(j);
                if (m) {
                    idx_t exp = n / j;
                    big_t term = pow3[exp];
                    if (m > 0)
                        sum += term;
                    else
                        sum -= term;
                }
            }
        }

        free(pow3);

        /* Final division.  sum is guaranteed divisible by (3*n). */
        big_t denom = (big_t)3 * (big_t)n;
        big_t quotient = sum / denom;
        return (seq_t)quotient;
    } else {
        /* seq_t supports arbitrary precision (e.g. a big integer class). */
        /* We accumlate the sum in the native seq_t type.  */
        seq_t sum = 0;

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i != 0)
                continue;

            if (i % 3 != 0) {
                int m = mu_of(i);
                if (m) {
                    seq_t term = pow3(n / i);      /* 3^(n/i) */
                    if (m > 0)
                        sum += term;
                    else
                        sum -= term;
                }
            }

            idx_t j = n / i;
            if (j != i && j % 3 != 0) {
                int m = mu_of(j);
                if (m) {
                    seq_t term = pow3(n / j);
                    if (m > 0)
                        sum += term;
                    else
                        sum -= term;
                }
            }
        }

        seq_t denom = (seq_t)3 * (seq_t)n;
        return sum / denom;
    }
}
