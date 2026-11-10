/*
 * A074043  Denominator of Sum_{k=1..n} 1/A077800(k), numerator=A074042.
 * https://oeis.org/A074043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/A077800(k) in lowest terms,
 *            where A077800 lists the twin primes 3,5,5,7,11,13,... (computed
 *            by a sieve plus exact fraction addition)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

#define A074043_SIEVE_LIMIT 1000000
#define A074043_MAX_TERMS   20000

static seq_t  A074043_twin_terms[A074043_MAX_TERMS];
static size_t A074043_twin_count = 0;
static uint8_t A074043_sieve[A074043_SIEVE_LIMIT + 1];

static void A074043_build(void)
{
    static int built = 0;
    if (built) return;
    built = 1;
    size_t i, j;
    A074043_sieve[0] = A074043_sieve[1] = 1;
    for (i = 2; i * i <= A074043_SIEVE_LIMIT; ++i) {
        if (!A074043_sieve[i]) {
            for (j = i * i; j <= A074043_SIEVE_LIMIT; j += i) A074043_sieve[j] = 1;
        }
    }
    size_t prev = 0;
    for (i = 2; i <= A074043_SIEVE_LIMIT && A074043_twin_count < A074043_MAX_TERMS; ++i) {
        if (!A074043_sieve[i]) {
            if (prev && i - prev == 2) {
                A074043_twin_terms[A074043_twin_count++] = (seq_t)prev;
                if (A074043_twin_count < A074043_MAX_TERMS)
                    A074043_twin_terms[A074043_twin_count++] = (seq_t)i;
            }
            prev = i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A074043_build();
    seq_t num = 0, den = 1;
    for (idx_t k = 1; k <= n; ++k) {
        if ((size_t)(k - 1) >= A074043_twin_count) break;
        seq_t p = A074043_twin_terms[(size_t)(k - 1)];
        if (p == 0) break;
        if (den % p == 0) {
            num = num + den / p;
        } else {
            num = num * p + den;
            den = den * p;
        }
        seq_t g = gcd(num, den);
        if (g > 1) {
            num = num / g;
            den = den / g;
        }
    }
    return den;
}
