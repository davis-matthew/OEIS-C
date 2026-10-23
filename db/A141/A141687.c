/*
 * A141687  Triangle read by rows: t(n,m) = 1 - ((prime(n) - prime(m))/2 mod
 *          2).
 * https://oeis.org/A141687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows * t(n,m) = 1 - ((prime(n) -
 *            prime(m)) / 2 mod 2) * where prime(k) is the k‑th odd prime
 *            (3,5,7,11,...). * a(n) is the n‑th entry in the row‑major
 *            flattening of the triangle. * 1-indexed: a(1)=1, a(2)=0, a(3)=1,
 *            a(4)=1,
 * Range:     u8 n <= 4950, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   _rc_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_rc_from_index.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Dynamic array of odd primes (3,5,7,11,...)                           */
static uint32_t *odd_primes   = NULL;
static idx_t     odd_cnt      = 0;           /* number of primes stored */

/* Test primality: trial division by known primes up to sqrt(x) */
static bool _is_prime(uint32_t x)
{
    if (x < 2)
        return false;
    for (idx_t i = 0; i < odd_cnt; ++i) {
        uint32_t p = odd_primes[i];
        if ((uint64_t)p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Ensure we have at least k odd primes stored */
static void _ensure_odd_primes(idx_t k)
{
    if (odd_cnt >= k)
        return;
    /* grow array */
    size_t new_cap = odd_cnt ? odd_cnt << 1 : 16;
    if (new_cap < k)
        new_cap = k;
    if (odd_primes == NULL) {
        odd_primes = (uint32_t*)malloc(new_cap * sizeof(uint32_t));
        if (odd_primes == NULL) exit(1);
        odd_primes[0] = 3;            /* first odd prime */
        odd_cnt = 1;
    } else {
        odd_primes = (uint32_t*)realloc(odd_primes, new_cap * sizeof(uint32_t));
        if (odd_primes == NULL) exit(1);
    }
    uint32_t candidate = odd_primes[odd_cnt - 1] + 2; /* next odd after last */
    while (odd_cnt < k) {
        if (_is_prime(candidate))
            odd_primes[odd_cnt++] = candidate;
        candidate += 2;
    }
}

/* Get the k-th odd prime (1‑based indexing) */
static uint32_t _odd_prime(idx_t k)   /* k>=1 */
{
    _ensure_odd_primes(k);
    return odd_primes[k - 1];
}

/* ------------------------------------------------------------------ */
/* Main function required by the test harness                         */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* out of bounds, not used */
    idx_t r, c;
    _rc_from_index(n, &r, &c);        /* r,c are 1‑based */

    uint32_t p_r = _odd_prime(r);     /* prime(r) (odd primes) */
    uint32_t p_c = _odd_prime(c);     /* prime(c) */

    uint64_t diff = (uint64_t)p_r - (uint64_t)p_c;   /* diff is even */
    uint64_t q    = diff / 2;                /* integer quotient */
    seq_t val = (seq_t)((q & 1U) ? 0 : 1);   /* 1 - (q%2) */

    return val;
}
