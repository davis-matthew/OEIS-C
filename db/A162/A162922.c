/*
 * A162922  Square array read by antidiagonals: m is the a(n,m)th positive
 *          integer among those which are coprime to n. Or a(n,m) = 0 if m and
 *          n are not coprime.
 * https://oeis.org/A162922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the n‑th element of the square array read along
 *            antidiagonals. * Array definition: a(n,m) = 0 if gcd(n,m)≠1,
 *            otherwise * a(n,m) = #{k : 1 ≤ k ≤ m and gcd(k,n)=1}. (i.e. the
 *            count of integers ≤ m that are coprime to n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_idx_t.h"

/* factorize n into distinct prime factors, stored in primes[0..cnt-1] */
static void factorize(idx_t n, uint64_t *primes, size_t *cnt)
{
    *cnt = 0;
    if (n % 2 == 0) {
        primes[(*cnt)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[(*cnt)++] = n;
}

/* count integers ≤ m that are coprime to n using inclusion–exclusion */
static uint64_t count_coprime(idx_t n, idx_t m)
{
    uint64_t primes[16];
    size_t pcnt;
    factorize(n, primes, &pcnt);

    if (pcnt == 0) return m;          /* n == 1 */
    uint64_t ans = m;
    uint64_t total = 1ULL << pcnt;    /* 2^pcnt */
    for (uint64_t mask = 1; mask < total; ++mask) {
        uint64_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                ++bits;
                prod *= primes[i];
            }
        }
        uint64_t term = m / prod;
        if (bits & 1) ans -= term;
        else          ans += term;
    }
    return ans;
}

/* binary search for least S with S*(S-1)/2 >= k */
static idx_t find_S(idx_t k)
{
    idx_t low = 1, high = 1;
    /* grow high until condition holds */
    while (1) {
        __int128 prod = (__int128)high * (high - 1) / 2;
        if (prod >= k) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 prod = (__int128)mid * (mid - 1) / 2;
        if (prod >= k)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* map linear index n to (i,j) in the array */
    idx_t S = find_S(n);
    __int128 prev_tri = (__int128)(S - 1) * (S - 2) / 2;
    idx_t offset = n - (idx_t)prev_tri;          /* 1‑based position inside diagonal */
    idx_t i = S - offset;
    idx_t j = offset;

    if (gcd_idx_t(i, j) != 1)
        return (seq_t)0;

    uint64_t val = count_coprime(i, j);
    return (seq_t)val;
}
