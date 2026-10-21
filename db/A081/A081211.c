/*
 * A081211  a(n) = A081210(A081210(n)).
 * https://oeis.org/A081211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A081210(A081210(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple factorisation of a 64‑bit integer.  On return
 * `pr[i]` holds the i‑th prime factor and `ex[i]` the exponent.
 * `cnt` is set to the number of distinct prime factors. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) {
            ++e;
            n /= 2;
        }
        pr[c] = 2;
        ex[c] = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                ++e;
                n /= p;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Test whether a 64‑bit integer is squarefree: no prime
 * divides it with multiplicity ≥ 2.  We simply test all
 * squares d² <= n. */
static bool is_squarefree_uint64(uint64_t n)
{
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % (d * d) == 0)
            return false;
    }
    return true;
}

/* The function A081210 applied to a 64‑bit integer.
 * It returns a value that fits into seq_t (assumed to be
 * able to hold the result). */
static seq_t A081210_uint64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)n;

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_uint64(n, primes, exps, &cnt);

    uint64_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t m;
        if (e == 1) {
            m = p;
        } else {
            /* compute p^e */
            uint64_t pe = 1;
            for (uint64_t j = 0; j < e; ++j)
                pe *= p;

            /* largest squarefree <= p^e */
            uint64_t k = pe;
            while (!is_squarefree_uint64(k))
                --k;
            m = k;
        }
        res *= m;
    }

    return (seq_t)res;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    x = (uint64_t)A081210_uint64(x);
    x = (uint64_t)A081210_uint64(x);
    return (seq_t)x;
}
