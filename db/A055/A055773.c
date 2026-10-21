/*
 * A055773  a(n) = Product_{p in P_n} where P_n = {p prime, n/2 < p <= n }.
 * https://oeis.org/A055773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏{ p prime | n/2 < p ≤ n } (OEIS A055773)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 52, u64 n <= 100,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --- local integer‑square‑root (avoid external helper) --- */
static uint64_t _floor_isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    /* binary search on [1, 2^32], safe for 64‑bit values */
    uint64_t lo = 1, hi = 1ULL << 32;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --- dynamic–array cache of primes up to the largest n reached --- */
static uint64_t *prime_buf = NULL;   /* array of known primes      */
static size_t   prime_cnt = 0;       /* number of primes stored    */
static size_t   prime_cap = 0;       /* current capacity           */

/* add primes up to and including 'limit' to prime_buf */
static void
ensure_primes(idx_t limit)
{
    if (limit < 2) return;           /* nothing to add */

    idx_t start = prime_cnt ? prime_buf[prime_cnt - 1] + 1 : 2;

    for (idx_t cand = start; cand <= limit; ++cand) {
        uint64_t p = (uint64_t)cand;
        uint64_t sqrt_p = _floor_isqrt_u64(p);

        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && prime_buf[i] <= sqrt_p; ++i) {
            if (p % prime_buf[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* grow buffer if needed */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t*)realloc(prime_buf, new_cap * sizeof(uint64_t));
                if (!tmp) abort();     /* unrecoverable */
                prime_buf = tmp;
                prime_cap = new_cap;
            }
            prime_buf[prime_cnt++] = p;
        }
    }
}

seq_t
a(idx_t n)
{
    /* gather all primes up to n */
    ensure_primes(n);

    seq_t product = 1;
    idx_t half = n >> 1;   /* floor(n/2) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_buf[i];
        if (p > half && p <= n)
            product *= (seq_t)p;
        else if (p > n)
            break;          /* primes are sorted */
    }
    return product;
}
