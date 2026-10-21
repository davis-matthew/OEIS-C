/*
 * A072027  Swap (2,3) and all twin prime pairs >(3,5) in prime factorization
 *          of n.
 * https://oeis.org/A072027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072027:  a(n) = product over prime powers p^e of a(p)^e
 *  where a(p) = 5-p for p<=3,
 *  a(p) = p+2 if p+2 is prime,
 *  a(p) = p-2 if p-2 is prime,
 *  a(p) = p otherwise.  The function is multiplicative.           */

#include <stdint.h>
#include <stddef.h>

/* Simple primality test by trial division – fast enough for n <= 10⁶. */
static int is_prime_u64(uint64_t n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return (n == 2);
    if (n % 3 == 0) return (n == 3);
    for (uint64_t d = 5; d <= n / d; d += 6) {
        if (n % d == 0) return 0;
        if (n % (d + 2) == 0) return 0;
    }
    return 1;
}

/* Factor n into distinct primes (pr[]) and exponents (exp[]).   */
/* cnt is set to the number of distinct primes found.           */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt) {
    size_t i = 0;
    uint64_t c;

    if (n <= 1) {
        *cnt = 0;
        return;
    }

    /* factor 2 */
    c = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        c++;
    }
    if (c) {
        pr[i] = 2;
        exp[i] = c;
        i++;
    }

    /* odd factors */
    for (uint64_t f = 3; f <= n / f; f += 2) {
        if (n % f == 0) {
            c = 0;
            while (n % f == 0) {
                n /= f;
                c++;
            }
            pr[i] = f;
            exp[i] = c;
            i++;
        }
    }
    if (n > 1) {          /* remaining prime */
        pr[i] = n;
        exp[i] = 1;
        i++;
    }
    *cnt = i;
}

seq_t a(idx_t n) {
    /* Handle the first term a(1) = 1. */
    if (n == 1u) return (seq_t)1;

    /* Factor n.  n is at most 64 bit, so this is efficient. */
    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; i++) {
        uint64_t p = pr[i];
        uint64_t mapped;

        if (p <= 3) {
            mapped = 5 - p;               /* 2→3, 3→2 */
        } else {
            if (is_prime_u64(p + 2)) {
                mapped = p + 2;
            } else if (p > 2 && is_prime_u64(p - 2)) {
                mapped = p - 2;
            } else {
                mapped = p;
            }
        }

        /* Multiply result by mapped^exp[i] (simple exponentiation). */
        for (uint64_t e = 0; e < exp[i]; e++) {
            result *= (seq_t)mapped;
        }
    }
    return result;
}
