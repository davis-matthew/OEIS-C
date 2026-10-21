/*
 * A219606  Prime gaps and primes interleaved.
 * https://oeis.org/A219606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219606: a(2k)   = kth prime, a(2k+1) = p_{k+2} - p_{k+1} */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Prime cache: primes[i] == (i+1)-th prime (primes[0] = p1). */
static uint64_t *primes      = NULL;
static size_t    primes_cnt  = 0;   /* number of primes stored */
static size_t    primes_cap  = 0;   /* buffer capacity   */
static uint64_t  last_checked = 1;  /* largest number already tested for primality */

/* Extend the prime table until it contains at least 'needed' primes. */
static void ensure_primes(idx_t needed)
{
    if (primes_cnt >= needed)
        return;                 /* already sufficient */

    uint64_t cand = last_checked + 1;
    while (primes_cnt < needed) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;         /* no divisor found up to sqrt(cand) */
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {          /* grow the buffer */
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_cnt++] = cand;
        }
        ++cand;
    }
    last_checked = cand - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                 /* unused offset position */

    if ((n & 1) == 0) {            /* even index: return the n/2‑th prime */
        idx_t k = n >> 1;
        if (k == 0)
            return 0;              /* safety, should never happen for n ≥ 2 */
        ensure_primes(k);
        return (seq_t)primes[k - 1];   /* primes[0] is p1 */
    } else {                        /* odd index: return the gap */
        idx_t m = (n - 1) >> 1;      /* m = (n-1)/2   */
        ensure_primes(m + 2);        /* need p_{m+2} and p_{m+1} */
        size_t idx0 = (size_t)m;     /* p_{m+1}  */
        size_t idx1 = idx0 + 1;      /* p_{m+2}  */
        uint64_t gap = primes[idx1] - primes[idx0];
        return (seq_t)gap;
    }
}
