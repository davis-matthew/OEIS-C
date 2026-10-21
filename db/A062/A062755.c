/*
 * A062755  a(n) = sigma_n(n^2): sum of n-th powers of divisors of n^2.
 * https://oeis.org/A062755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum_{d | n^2} d^n (OEIS A062755)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64, pow_uint64_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/pow_uint64_small.h"

/* generic exponential applied to the sequence type */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    /* If seq_t fits in a machine word, use 128‑bit arithmetic to avoid overflow
       otherwise delegate to the arbitrary‑precision routine. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 res = 1;
        unsigned __int128 b   = (unsigned __int128)base;
        while (exp) {
            if (exp & 1U) res *= b;
            b *= b;
            exp >>= 1U;
        }
        return (seq_t)res;
    } else {
        /* seq_t is an arbitrary‑precision type */
        return big_pow(base, (seq_t)exp);
    }
}

/* Recursive generation of all divisors of n^2 and accumulation of the sum. */
static void gen_divisors(idx_t          idx,
                         uint64_t       cur,
                         const uint64_t primes[],
                         const uint64_t exps[],
                         size_t         cnt,
                         seq_t          *sum,
                         idx_t          n)
{
    if (idx == cnt) {                      /* finished a divisor */
        *sum += pow_seq((seq_t)cur, n);
        return;
    }

    uint64_t p      = primes[idx];
    uint64_t e      = exps[idx];
    uint64_t bmax   = 2 * e;                /* exponent range for n^2 */
    uint64_t val    = 1;                    /* p^b */

    for (uint64_t b = 0; b <= bmax; ++b) {
        gen_divisors(idx + 1, cur * val, primes, exps, cnt, sum, n);
        val *= p;                           /* next power of p */
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n must be >= 1 for the OEIS indexing */
    if (n == 0) return 0;   /* defined for safety */

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum over all divisors of n^2 */
    seq_t sum = 0;
    gen_divisors(0, 1, primes, exps, cnt, &sum, n);
    return sum;
}
