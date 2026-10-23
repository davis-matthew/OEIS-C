/*
 * A166865  Triangle read by rows: R(n,k)=prime(k)^(prime(n)-2) mod n,
 *          1<=k<=n.
 * https://oeis.org/A166865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   R(m,n) = prime(n)^(prime(m)-2) mod m (triangle read by rows)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(t) is the t‑th element of the triangle read row‑by‑row. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static inline __int128 triangular(idx_t x)
{
    return (__int128)x * (x + 1) / 2;
}

/* -------------------- prime sieve helpers -------------------- */
static uint64_t *primes   = NULL;   /* primes[0] = 2 = prime(1) */
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;
static uint64_t prime_candidate = 2;

static void ensure_primes(size_t need)
{
    if (primes_cnt >= need) return;

    /* Grow the prime array */
    size_t new_cap = primes_cap ? primes_cap * 2 : 32;
    while (new_cap < need) new_cap <<= 1;
    if (new_cap > primes_cap) {
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();   /* allocation failed */
        primes    = tmp;
        primes_cap = new_cap;
    }

    /* Simple incremental sieve (trial division) */
    while (primes_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > prime_candidate) break;
            if (prime_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            primes[primes_cnt++] = prime_candidate;
        }
        ++prime_candidate;
    }
}

/* ----------------------- sequence function -------------------- */
seq_t a(idx_t t)
{
    if (t == 0) return 0;          /* undefined for offset 1; return 0 */

    /* Find the row r such that r(r+1)/2 >= t */
    idx_t lo = 1, hi = 1;
    while (triangular(hi) < t) hi <<= 1;          /* double hi until enough */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= t) hi = mid;
        else                      lo = mid + 1;
    }
    idx_t r = lo;               /* row index (1‑based) */
    idx_t prev_sum = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k  = t - prev_sum;    /* column position (1‑based) */

    /* Prime numbers needed */
    ensure_primes((size_t)(r > k ? r : k));

    uint64_t prime_r = primes[r - 1];
    uint64_t prime_k = primes[k - 1];
    uint64_t exponent = prime_r - 2;   /* prime_r >= 2 for r >= 1 */

    seq_t base  = (seq_t)prime_k;
    seq_t mod   = (seq_t)r;
    seq_t res   = mod_pow(base, exponent, mod);   /* prime_k^exponent mod r */

    return res;
}
