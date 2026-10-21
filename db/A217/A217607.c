/*
 * A217607  Smallest k > 1 such that n divides binomial(n,k).
 * https://oeis.org/A217607
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k>1 such that n divides binomial(n,k)
 * Range:     u8 n <= 9799, u16 n <= 9799, u32 n <= 9799, u64 n <= 9799,
 *            u128 n <= 9799, big any n
 * Helpers:   v_p_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/v_p_fact.h"

/* Factorises n; primes[] and exps[] store the prime factors and their exponents.
   The number of distinct factors is returned through *cnt. */
static void factor(idx_t n, idx_t primes[16], idx_t exps[16], idx_t *cnt)
{
    idx_t c = 0;

    if ((n & 1ULL) == 0) {
        primes[c] = 2;
        exps[c] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exps[c]++;
        }
        c++;
    }

    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[c] = p;
            exps[c] = 0;
            while (n % p == 0) {
                n /= p;
                exps[c]++;
            }
            c++;
        }
    }

    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        c++;
    }

    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 3) /* behavior below offset is irrelevant */
        return 0;

    idx_t primes[16], exps[16], pcnt;
    factor(n, primes, exps, &pcnt);

    /* v_p(n!) for each prime factor */
    idx_t vN[16];
    for (idx_t i = 0; i < pcnt; ++i)
        vN[i] = v_p_fact(n, primes[i]);

    for (idx_t k = 2; k <= n - 1; ++k) {
        bool ok = true;
        for (idx_t i = 0; i < pcnt; ++i) {
            idx_t vp = vN[i] - v_p_fact(k, primes[i]) - v_p_fact(n - k, primes[i]);
            if (vp < exps[i]) { ok = false; break; }
        }
        if (ok)
            return (seq_t)k;
    }

    /* Theoretical guarantee: k = n-1 always works, but control never reaches here */
    return (seq_t)(n - 1);
}
