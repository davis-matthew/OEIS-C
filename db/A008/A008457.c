/*
 * A008457  a(n) = Sum_{ d >= 1, d divides n} (-1)^(n-d)*d^3.
 * https://oeis.org/A008457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(n-d) * d^3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1559, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void gen_divisors(const uint64_t *primes, const int *exps,
                         int idx, int cnt,
                         idx_t cur, idx_t n, int64_t *total)
{
    if (idx == cnt) {
        int parity = ((n - cur) & 1);
        int64_t cube = (int64_t)cur * cur * cur;
        if (parity) *total -= cube;
        else      *total += cube;
        return;
    }
    uint64_t p = primes[idx];
    idx_t prod = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors(primes, exps, idx + 1, cnt, prod, n, total);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    /* Factor n */
    uint64_t primes[20];
    int exps[20];
    int cnt = 0;
    idx_t temp = n;
    for (int p = 2; (idx_t)p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = (uint64_t)p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Sum over divisors */
    int64_t total = 0;
    gen_divisors(primes, exps, 0, cnt, (idx_t)1, n, &total);

    return (seq_t)total;
}
