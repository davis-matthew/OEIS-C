/*
 * A191906  The remainder of (product of proper divisors of n) mod (sum of
 *          proper divisors of n).
 * https://oeis.org/A191906
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A007956(n) mod A001065(n)
 * Range:     u8 n <= 219, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007956(n) = product of proper divisors of n */
/* A001065(n) = sum of proper divisors of n */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* Multiply two numbers modulo mod, safe for arbitrary‑size seq_t */
static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    if (mod == (seq_t)0) return (seq_t)0;
    seq_t result = 0;
    a %= mod;
    while (b) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}

/* Recursively generate all divisors of n (including 1 and n) */
static void gen_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t pcnt, uint64_t cur, uint64_t n,
                     seq_t mod, seq_t *prod)
{
    if (pcnt == 0) {
        if (cur != n)           /* skip n itself */
            *prod = mulmod(*prod, (seq_t)cur, mod);
        return;
    }
    uint64_t p = pr[0];
    uint64_t e = ex[0];
    uint64_t next = cur;
    for (uint64_t k = 0; k <= e; ++k) {
        gen_divs(pr + 1, ex + 1, pcnt - 1, next, n, mod, prod);
        next *= p;
    }
}

/* OEIS sequence A191906 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* offset is 2; behaviour below offset is irrelevant */

    /* Factor n */
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(N, primes, exps, &pcnt);

    /* Sum of all divisors of n */
    seq_t sigma = sigma_of(N);

    /* Sum of proper divisors */
    seq_t sum_proper = sigma - (seq_t)N;
    if (sum_proper == (seq_t)0) return 0;   /* should not happen for n >= 2 */

    /* Product of proper divisors modulo sum_proper */
    seq_t prod = 1 % sum_proper;            /* handle the special case mod == 1 */
    gen_divs(primes, exps, pcnt, 1, N, sum_proper, &prod);

    return prod;
}
