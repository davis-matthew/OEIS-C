/*
 * A143614  Triangle read by rows: A054521 * A051731 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A143614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = #{ j : 1 ≤ j ≤ r, j%k==0, gcd(r,j)==1 }, where r is the
 *            row such that triangular(r-1) < n ≤ triangular(r), and k = n –
 *            triangular(r-1). The entry is 0 if gcd(r,k)≠1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _smallest_prime_factor, gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx_t.h"
#include "../utils/_smallest_prime_factor.h"

/* Count integers 1 ≤ m ≤ M that are coprime to n
   given n's distinct prime factors in 'primes' of length np. */
static seq_t coprime_count(idx_t n, idx_t M, const uint64_t *primes, size_t np)
{
    uint64_t cnt = M;
    size_t total_masks = (size_t)1 << np;
    for (size_t mask = 1; mask < total_masks; ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (size_t i = 0; i < np; ++i) {
            if (mask & ((size_t)1 << i)) {
                d *= primes[i];
                ++bits;
            }
        }
        uint64_t term = M / d;
        if (bits & 1)
            cnt -= term;
        else
            cnt += term;
    }
    return (seq_t)cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find row r with triangular(r-1) < n ≤ triangular(r) */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;          /* column index (1‑based) */

    /* If gcd(r,k) ≠ 1, the entry is 0 */
    if (gcd_idx_t(r, k) != 1)
        return (seq_t)0;

    /* Factor r into distinct prime factors */
    uint64_t prime_buf[64];
    size_t np = 0;
    uint64_t x = r;
    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);
        prime_buf[np++] = p;
        while (x % p == 0)
            x /= p;
    }

    idx_t M = r / k;                  /* floor(r / k) */
    return coprime_count(r, M, prime_buf, np);
}
