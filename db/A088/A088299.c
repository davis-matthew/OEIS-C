/*
 * A088299  Values of x + 2y, where x^2 + xy + y^2=p (x<y) is a prime of the
 *          form 6n + 1 (=A002476).
 * https://oeis.org/A088299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x + 2*y for the n-th prime p ≡ 1 (mod 6) that satisfies
 *            x² + x*y + y² = p with 0 < x < y. The sequence is indexed
 *            starting at n = 1, so a(1) corresponds to the first such prime
 *            (7)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- small helpers ---------- */

/* integer square root for uint64_t (floor) */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* value of x + 2*y for a prime p ≡ 1 (mod 6) */
static seq_t compute_value(uint64_t p)
{
    /* search increasing x until the quadratic discriminant 4p – 3x² is
       non‑negative and a perfect square.  For a prime of this form a
       unique solution with 0 < x < y always exists. */
    for (uint64_t x = 1;; ++x) {
        uint64_t d = 4ULL * p - 3ULL * x * x;
        if (d == 0) /* only possible for small primes we ignore */
            continue;
        if (d < 3ULL * x * x) break; /* discriminant became negative */

        uint64_t sqrt_d = isqrt_u64(d);
        if (sqrt_d * sqrt_d != d)
            continue;
        /* (sqrt_d - x) must be even to give integer y */
        if (((sqrt_d - x) & 1ULL) != 0)
            continue;
        uint64_t y = (sqrt_d - x) >> 1;
        if (x < y && y > 0) {
            /* cast to seq_t – it is safe because results fit in 64‑bit
               for the range of n we support */
            return (seq_t)x + (seq_t)2 * (seq_t)y;
        }
    }
    return 0; /* should never happen */
}

/* ---------- cache and prime generation ---------- */

static seq_t *seq_cache = NULL;     /* a(n) values, 0‑based */
static size_t cache_capacity = 0;
static idx_t cache_size = 0;       /* number of values stored */

/* list of primes for trial division (used for generating 1‑mod‑6 primes) */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;

/* last prime candidate processed (always odd, > 2) */
static uint64_t next_candidate = 7;

/* extend the cache to at least n entries */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* grow the value cache */
    size_t need = (size_t)n;
    if (need > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_capacity = new_cap;
    }

    /* initialise the prime list for trial division, if needed */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        /* small primes */
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_list[2] = 5;
        prime_cnt = 3;
    }

    /* generate primes until we have processed n 1‑mod‑6 primes */
    while (cache_size < n) {
        /* trial division to test primality of next_candidate */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* extend the prime list */
            if (prime_cnt + 1 > prime_cap) {
                size_t new_cap = prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = next_candidate;

            if ((next_candidate % 6) == 1) {
                seq_cache[cache_size++] = compute_value(next_candidate);
            }
        }

        /* move to the next odd candidate */
        next_candidate += 2;
    }
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 0 */
    ensure_cache(n);
    return seq_cache[n - 1];    /* 1‑based indexing for sequence */
}
