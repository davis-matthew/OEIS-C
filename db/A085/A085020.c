/*
 * A085020  a(n) = Sum_{d|n, (d+1) prime} (d + 1).
 * https://oeis.org/A085020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n, (d+1) prime} (d + 1)
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently  a(n) = Sum_{p prime, (p-1) | n} p) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static void gen_divs(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                     size_t idx, uint64_t cur, seq_t *sum)
{
    if (idx == cnt) {
        uint64_t p = cur + 1;
        if (is_prime(p))
            *sum += (seq_t)p;
        return;
    }
    uint64_t prime = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        gen_divs(pr, exp, cnt, idx + 1, cur * val, sum);
        val *= prime;
    }
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n == 0)
        return total;   /* safe guard – offset for OEIS starts at 1 */

    uint64_t n64 = (uint64_t)n;
    /* Factor n.  At most 64 prime factors for a 64‑bit number. */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt;
    factor_u64(n64, pr, exp, &cnt);

    /* Enumerate all divisors d of n and add (d+1) if it is prime. */
    gen_divs(pr, exp, cnt, 0ULL, 1ULL, &total);

    return total;
}
