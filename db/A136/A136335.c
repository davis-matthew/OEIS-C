/*
 * A136335  Indices of the primes p for which the number of representations of
 *          p as the sum of a perfect prime power (A025475: q^e with e>1) and
 *          an integer k which is less than q exceeds one.
 * https://oeis.org/A136335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136335: indices of primes p for which the number of
   representations p = q^e + k  (e > 1, q prime, 0 ≤ k < q)
   exceeds one.  The function enumerates primes and counts the
   representations until the requested number of qualifying indices
   has been found.  Results are returned in the 1‑based prime index. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/* Small integer square‑root (floor) for 64‑bit unsigned values.         */
static inline uint64_t floor_isqrt_u64(uint64_t n) {
    uint64_t x = 0;
    uint64_t bit = ((uint64_t)1) << 62;          /* largest power <= sqrt(2^64-1) */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* -------------------------------------------------------------------- */
/* Prime storage.  primes[0] = 2 is guaranteed after first call. */
static uint64_t *primes      = NULL;   /* dynamic array of primes */
static size_t   primes_cnt   = 0;       /* number of primes stored   */
static size_t   primes_cap   = 0;       /* allocated capacity       */
static uint64_t last_prime   = 2;       /* last prime produced      */

/* Qualifying index storage (1‑based prime indices). */
static idx_t *qual_indices = NULL;  /* dynamic array of indices */
static size_t  qual_cnt    = 0;     /* number of qualifying indices */
static size_t  qual_cap    = 0;     /* allocated capacity         */

/* -------------------------------------------------------------------- */
/* Grow the primes array to hold at least 'needed' entries. */
static void grow_primes(size_t needed) {
    if (primes_cap >= needed) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

/* Grow the qualifier array to hold at least 'needed' entries. */
static void grow_quals(size_t needed) {
    if (qual_cap >= needed) return;
    size_t new_cap = qual_cap ? qual_cap * 2 : 32;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(qual_indices, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    qual_indices = tmp;
    qual_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/* Simple primality test using the already stored primes. */
static bool is_prime_candidate(uint64_t n) {
    if (n < 2) return false;
    uint64_t limit = floor_isqrt_u64(n);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Produce the next prime after 'last_prime' and store it. */
static void generate_next_prime(void) {
    /* Ensure at least one prime (2) is present. */
    if (primes_cnt == 0) {
        grow_primes(1);
        primes[0] = 2;
        primes_cnt = 1;
        last_prime = 2;
        return;
    }

    uint64_t cand = last_prime + 1;
    if (cand <= 2) cand = 2;
    if ((cand & 1ULL) == 0) ++cand;          /* skip even numbers after 2 */

    for (;;) {
        if (is_prime_candidate(cand)) {
            ++primes_cnt;
            grow_primes(primes_cnt);
            primes[primes_cnt - 1] = cand;
            last_prime = cand;
            return;
        }
        cand += 2;                           /* skip even candidates */
    }
}

/* -------------------------------------------------------------------- */
/* Count representations of prime p as q^e + k (e>1, q prime,
   0 ≤ k < q); stop after two representations. */
static uint8_t count_reps(uint64_t p) {
    uint8_t reps = 0;
    uint64_t limit = floor_isqrt_u64(p - 1);
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t q = primes[i];
        uint64_t pow = q * q;               /* q^2 */
        while (pow <= p - 1) {
            uint64_t k = p - pow;
            if (k < q) {
                ++reps;
                if (reps > 1) return reps;
            }
            if (pow > (p - 1) / q) break;    /* avoid overflow */
            pow *= q;
        }
    }
    return reps;
}

/* -------------------------------------------------------------------- */
/* Ensure that at least 'needed' qualifying indices have been computed. */
static void ensure_qualifiers_up_to(idx_t needed) {
    if (needed == 0) return;
    while (qual_cnt < needed) {
        generate_next_prime();
        if (count_reps(primes[primes_cnt - 1]) > 1) {
            ++qual_cnt;
            grow_quals(qual_cnt);
            qual_indices[qual_cnt - 1] = (idx_t)primes_cnt;   /* 1‑based index */
        }
    }
}

/* -------------------------------------------------------------------- */
/* Public sequence function.  a(1) is the first qualifying index. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* undefined, safe fallback */
    ensure_qualifiers_up_to(n);
    return (seq_t)qual_indices[n - 1];
}
