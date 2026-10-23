/*
 * A024884  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = A023531, t = (composite numbers).
 * https://oeis.org/A024884
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A024884
   Formula: a(n) = Σ_{i=1..⌊n/2⌋} s(i) * t(n‑i+1)
   where s(i) = 1 if 8*i+9 is a perfect square, otherwise 0
   and t(j) is the j‑th composite number (t(1)=4, t(2)=6, …).
   This implementation builds the needed composites and primes incrementally,
   memoising them across calls.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

/* memoised tables ----------------------------------------------------- */

static seq_t *composites      = NULL;
static idx_t  composites_size = 0;
static idx_t  composites_cap  = 0;

static uint64_t *primes      = NULL;
static size_t   primes_size  = 0;
static size_t   primes_cap   = 0;

/* next integer to examine for primality */
static idx_t next_candidate = 4;

/* utility to grow an array of uint64_t ------------------------------- */
static void grow_uint64(uint64_t **arr, size_t *sz, size_t *cap)
{
    if (*cap == 0) {
        *cap = 64;
        *arr = (uint64_t *)malloc(*cap * sizeof(uint64_t));
    } else {
        *cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
    }
}

/* utility to grow an array of seq_t ---------------------------------- */
static void grow_seq_t(seq_t **arr, size_t *sz, size_t *cap)
{
    if (*cap == 0) {
        *cap = 64;
        *arr = (seq_t *)malloc(*cap * sizeof(seq_t));
    } else {
        *cap <<= 1;
        *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* very small primality test using the stored primes ------------------ */
static bool is_prime_uint64(idx_t n)
{
    if (n < 2) return false;
    /* small primes handled by the primes array (2,3 etc.) */
    for (size_t j = 0; j < primes_size; ++j) {
        uint64_t p = primes[j];
        if ((idx_t)p * (idx_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* make sure we have at least 'need' composites stored ---------------- */
static void ensure_composites(idx_t need)
{
    if (primes_cap == 0) {                 /* initialise primes array */
        grow_uint64(&primes, &primes_size, &primes_cap);
        /* insert first two primes */
        primes[0] = 2;
        primes[1] = 3;
        primes_size = 2;
    }

    while (composites_size < need) {
        idx_t cand = next_candidate++;
        if (is_prime_uint64(cand)) {
            /* store new prime */
            if (primes_size == primes_cap) grow_uint64(&primes, &primes_size, &primes_cap);
            primes[primes_size++] = cand;
        } else {
            /* store new composite */
            if (composites_size == composites_cap) grow_seq_t(&composites, &composites_size, &composites_cap);
            composites[composites_size++] = (seq_t)cand;
        }
    }
}

/* s(i): 1 if 8*i+9 is a perfect square -------------------------------- */
static inline int s_value(idx_t i)
{
    idx_t v = 8 * i + 9;
    idx_t r = isqrt_u64(v);
    return (r * r == v) ? 1 : 0;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence offset is 2 */

    ensure_composites(n);         /* need composites[0..n-1] */

    idx_t k = n / 2;
    seq_t sum = 0;

    for (idx_t i = 1; i <= k; ++i) {
        int si = s_value(i);
        if (si) {
            /* composite_{n-i+1} = composites[n-i] (0‑based) */
            sum += composites[n - i];
        }
    }
    return sum;
}
