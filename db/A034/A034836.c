/*
 * A034836  Number of ways to write n as n = x*y*z with 1 <= x <= y <= z.
 * https://oeis.org/A034836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (np + 3·nsq + 2·ntp)/6
    where the signature of n is the multiset of exponents of its prime factorisation
    and
        np  = ∏ binomial(e+2,2)
        nsq = ∏ (⌊e/2⌋+1)
        ntp = 1  if every e is divisible by 3, else 0
    (C uses a trial‑division factorisation, which is fast enough for n≤10⁴.) */

#include <stdint.h>
#include <stddef.h>

/* factorises n into unique primes and their exponents
   primes and exps must be arrays of size at least 64
   on return *cnt = number of distinct prime factors */
static void factor_u64(uint64_t n, uint64_t *primes, unsigned char *exps, size_t *cnt)
{
    size_t k = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            unsigned char e = 0;
            do {
                t /= p;
                ++e;
            } while (t % p == 0);
            primes[k] = p;
            exps[k]   = e;
            ++k;
        }
    }
    if (t > 1) {
        primes[k] = t;
        exps[k]   = 1;
        ++k;
    }
    *cnt = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset guard, not used by OEIS */

    uint64_t primes[64];
    unsigned char exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t np  = 1;             /* product of binomials */
    seq_t nsq = 1;             /* product for squares */
    seq_t ntp = 1;             /* 1 if every exponent divisible by 3, else 0 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        /* binomial(e+2,2) = (e+2)*(e+1)/2 */
        seq_t term = ((seq_t)(exps[i] + 2) * (seq_t)(exps[i] + 1)) / (seq_t)2;
        np *= term;
        nsq *= ((seq_t)(exps[i] / 2) + (seq_t)1);

        if (exps[i] % 3 != 0) ntp = 0;
    }

    /* final formula */
    return (np + (seq_t)3 * nsq + (seq_t)2 * ntp) / (seq_t)6;
}
