/*
 * A082729  Least positive number that can be written using all divisors of n
 *          and the operations add and subtract.
 * https://oeis.org/A082729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive value obtained by assigning each divisor
 *            of n * a + 1 or − 1 and summing. It equals 2·s_min − σ(n), where
 *            σ(n) is the sum * of all divisors of n and s_min is the smallest
 *            reachable subset sum that * is strictly larger than σ(n)/2
 * Range:     u8 n <= 256, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small helper: factorise  n into prime^exp pairs.
 * primes[0..c-1] and exps[0..c-1] contain the factorisation.
 * c <= 64 is sufficient for n ≤ 10^18. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively generate all divisors from the prime factorisation.
 * divs[0..max-1] receives the generated divisors, *divcnt is the count
 * after completion.  divs must be pre‑allocated with size >= number of
 * divisors. */
static void gen_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t pcnt, uint64_t current,
                         uint64_t *divs, size_t *divcnt)
{
    if (pcnt == 0) {
        divs[(*divcnt)++] = current;
        return;
    }
    uint64_t p = primes[0];
    uint64_t e = exps[0];
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divisors(primes + 1, exps + 1, pcnt - 1, current, divs, divcnt);
        current *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1; behaviour below is irrelevant */
        return 0;

    /* 1. factorise n. */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    /* 2. how many divisors? */
    size_t num_divs = 1;
    for (size_t i = 0; i < pcnt; ++i)
        num_divs *= (size_t)(exps[i] + 1);

    /* 3. generate all divisors. */
    uint64_t *divs = (uint64_t *)malloc(num_divs * sizeof(uint64_t));
    size_t divcnt = 0;
    gen_divisors(primes, exps, pcnt, 1, divs, &divcnt);

    /* 4. sum of divisors σ(n). */
    seq_t sigma = 0;
    for (size_t i = 0; i < divcnt; ++i)
        sigma += (seq_t)divs[i];

    /* 5. subset–sum DP up to σ(n)/2. */
    size_t tot = (size_t)sigma;                  /* safe: n ≤ 10 000 */
    unsigned char *reach = (unsigned char *)calloc(tot + 1, sizeof(unsigned char));
    reach[0] = 1;
    for (size_t i = 0; i < divcnt; ++i) {
        size_t d = (size_t)divs[i];
        if (d == 0) continue;
        for (size_t s = tot; s >= d; --s) {
            if (reach[s - d])
                reach[s] = 1;
            if (s == d) /* avoid wrap‑around of unsigned */
                break;
        }
    }

    /* 6. find smallest reachable sum strictly > σ(n)/2. */
    size_t half = tot / 2;
    size_t s_min = half + 1;
    for (; s_min <= tot; ++s_min) {
        if (reach[s_min])
            break;
    }

    /* 7. compute answer. */
    seq_t ans = (seq_t)((seq_t)2 * (seq_t)s_min - sigma);

    /* 8. clean up. */
    free(reach);
    free(divs);
    return ans;
}
