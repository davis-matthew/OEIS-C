/*
 * A225461  a(n) = (prime(k) + prime(k+1))/2 where k = A098015(n).
 * https://oeis.org/A225461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(k)+prime(k+1))/2 where k is the n‑th index i with
 *            p(i+1)-p(i)=2*prime
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static uint64_t *primes = NULL;          /* stored primes */
static size_t   primes_cnt = 0;          /* number of primes currently stored */
static size_t   primes_cap = 0;          /* capacity of the primes array */

static uint64_t *qual_k = NULL;         /* indices k of qualifying pairs */
static size_t   qual_cnt = 0;            /* count of qualifying indices found */
static size_t   qual_cap = 0;            /* capacity of the qual_k array */

/* Ensure that an array has capacity for at least `needed` elements. */
static inline void grow_array(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    ensure_capacity(needed, cap, ptr, elem_size);
}

/* Append a value to a dynamic array of uint64_t. */
static inline void append_uint64(uint64_t *val, uint64_t **array, size_t *cnt, size_t *cap)
{
    /* grow if necessary */
    if (*cnt >= *cap) {
        *cap = (*cap == 0) ? 4 : (*cap) * 2;
        *array = (uint64_t *)realloc(*array, *cap * sizeof(uint64_t));
    }
    (*array)[(*cnt)++] = *val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, safe return */

    /* Ensure we have enough qualifying pairs. */
    while (qual_cnt < (size_t)n) {
        /* Generate next prime. */
        uint64_t next_prime;
        if (primes_cnt == 0) {
            next_prime = 2;
        } else if (primes_cnt == 1) {
            next_prime = 3;
        } else {
            next_prime = primes[primes_cnt - 1] + 2;
            while (!is_prime((idx_t)next_prime)) {
                next_prime += 2;
            }
        }

        /* Append the new prime. */
        append_uint64(&next_prime, &primes, &primes_cnt, &primes_cap);

        /* If we have at least two primes, check the gap. */
        if (primes_cnt >= 2) {
            uint64_t prev = primes[primes_cnt - 2];
            uint64_t gap  = next_prime - prev;
            if ((gap & 1) == 0 && is_prime((idx_t)(gap >> 1))) {
                /* Qualifying index is the position of prev prime (1‑based). */
                uint64_t k = primes_cnt - 1;   /* because primes_cnt is 1‑based count */
                append_uint64(&k, &qual_k, &qual_cnt, &qual_cap);
            }
        }
    }

    /* Retrieve the n‑th qualifying index. */
    uint64_t k = qual_k[(size_t)n - 1];  /* 1‑based index into primes array */
    /* Compute (p(k)+p(k+1))/2.  primes array is 0‑based. */
    seq_t pk   = (seq_t)primes[k - 1];
    seq_t pk1  = (seq_t)primes[k];
    return (pk + pk1) / (seq_t)2;
}
