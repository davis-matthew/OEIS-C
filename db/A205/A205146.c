/*
 * A205146  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=prime(j)*prime(j+1).
 * https://oeis.org/A205146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3828, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205146
   a(n) = least k such that n | (s(k)-s(j)) for some 1≤j<k,
   where s(j) = prime(j)*prime(j+1).
   Offset 1.
   Implementation: generate primes on demand, then scan the
   residues s(j) mod n until a repeated residue is found,
   which yields the minimal k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime generation (naive, sufficient for n up to a few ten‑thousands) */
static uint32_t *primes    = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;

static void grow_primes_cap(void)
{
    if (primes_cap == 0) {
        primes_cap = 128;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    } else {
        primes_cap <<= 1;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

static void build_primes(size_t need)
{
    if (primes_cnt >= need) return;

    if (primes_cnt == 0) {
        grow_primes_cap();
        primes[primes_cnt++] = 2;          /* first prime */
    }

    uint32_t cand = primes[primes_cnt - 1] + 1;
    if ((cand & 1) == 0) cand++;          /* start with an odd candidate */

    while (primes_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) grow_primes_cap();
            primes[primes_cnt++] = cand;
        }
        cand += 2;
    }
}

/* ------------------------------------------------------------------ */
/* Core sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* undefined for offset 0 */

    /* We need primes up to index n+2 for the worst case j = n+1. */
    build_primes((size_t)(n + 2));

    /* Array of flags for residues modulo n. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!seen) abort();

    idx_t result = 0;
    for (idx_t j = 1;; ++j) {
        idx_t r = (idx_t)((((uint64_t)primes[j - 1] * (uint64_t)primes[j]) % n));
        if (seen[r]) {            /* first collision gives minimal k = j */
            result = j;
            break;
        }
        seen[r] = 1;
    }
    free(seen);

    return (seq_t)result;
}
