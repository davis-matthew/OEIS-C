/*
 * A181811  a(n) = smallest integer that, upon multiplying any divisor of n,
 *          produces a member of A025487.
 * https://oeis.org/A181811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A181811
   Formula: a(n) = ∏_{p^e‖n} (primorial( index(p)-1 ))^e
   where primorial(k) = ∏_{i=0}^{k-1} prime(i+1)
   (prime(1)=2, prime(2)=3, …).

   The implementation factors n, looks up each prime’s index,
   multiplies the appropriate primorial value, raises it to the
   factor exponent, and accumulates the final product. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* ---------- Prime list / cache ------------------------------------ */

static uint64_t *prime_list   = NULL;   /* primes[0] = 2, primes[1] = 3, …   */
static size_t   prime_count  = 0;       /* number of primes stored          */
static idx_t    prime_limit  = 0;       /* highest prime precomputed so far */

/* Ensure primes up to at least `limit` are in prime_list. */
static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;

    uint64_t *new_list   = NULL;
    size_t   new_count   = 0;

    build_primes_upto(limit, &new_list, &new_count);

    free(prime_list);
    prime_list  = new_list;
    prime_count = new_count;
    prime_limit = limit;
}

/* Binary search: return 1‑based index of prime p in prime_list. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;     /* hi is exclusive   */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    /* p must exist in the table. */
    return lo + 1;                        /* 1‑based */
}

/* ---------- Primitive exponentiation on seq_t --------------------- */

static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Return product of the first `k` primes (i.e. primorial(k)).
   k = 0 → 1. */
static seq_t primorial_of_idx(size_t k)
{
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < k && i < prime_count; ++i)
        prod *= (seq_t)prime_list[i];
    return prod;
}

/* ---------- Main sequence function --------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* out‑of‑bounds guard, though offset is 1 */

    /* Ensure we have enough primes for the factorisation. */
    ensure_prime_limit((idx_t)n);

    /* Factor n.  factor_u64 writes primes and exponents into the
       supplied arrays and sets *cnt to the number of distinct factors. */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t   fcnt = 0;
    factor_u64((uint64_t)n, pf, ex, &fcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t  p   = pf[i];
        unsigned  e   = (unsigned)ex[i];
        size_t    idx = prime_index(p);                 /* 1‑based */
        seq_t     base = primorial_of_idx(idx - 1);     /* A002110(idx‑1) */
        seq_t     power = pow_seq(base, e);
        result *= power;
    }

    return result;
}
