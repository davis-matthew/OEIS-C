/*
 * A039956  Even squarefree numbers.
 * https://oeis.org/A039956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th even squarefree number.
 * The sequence is the list of even numbers that are squarefree.
 * This implementation sieves up to a bound that is roughly
 * six times the desired index (a crude upper bound) and keeps
 * the results in a static array for reuse on subsequent calls.
 *
 * The code uses the helper build_primes_upto to obtain all primes
 * up to the square root of the sieve limit.  It then marks
 * composite numbers that contain a squared prime factor and
 * collects those that are even and unmarked.
 *
 * All allocations are cast appropriately for C++.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static state */
static seq_t *even_sqf = NULL;   /* array of even squarefree numbers */
static size_t even_len   = 0;     /* how many have been stored */
static size_t even_cap   = 0;     /* capacity of even_sqf */
static uint64_t sieve_lim = 0;    /* largest sieve limit computed so far */

/* ------------------------------------------------------------------ */
/* recompute_sieve:  build the list of even squarefree numbers up to */
static void recompute_sieve(uint64_t limit)
{
    /* --- get primes up to sqrt(limit) -------------------------------- */
    size_t p_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &p_cnt);  /* C++ safe, primes gets freed later */

    /* --- mark numbers that are NOT squarefree ----------------------- */
    uint8_t *non_sqf = (uint8_t *)calloc((size_t)(limit + 1), 1); /* C++ cast */
    if (!non_sqf) exit(1);

    for (size_t i = 0; i < p_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t sq = p * p;
        if (sq > limit) break;
        for (uint64_t j = sq; j <= limit; j += sq) {
            non_sqf[(size_t)j] = 1;
        }
    }
    free(primes);

    /* --- collect even squarefree numbers --------------------------- */
    size_t capacity = (size_t)(limit / 5) + 10;   /* overestimate for safety */
    seq_t *new_arr = (seq_t *)calloc(capacity, sizeof(seq_t));
    if (!new_arr) exit(1);
    size_t l = 0;

    for (uint64_t i = 1; i <= limit; ++i) {
        if (!non_sqf[(size_t)i] && ((i & 1ULL) == 0ULL)) {
            if (l >= capacity) {
                capacity *= 2;
                new_arr = (seq_t *)realloc(new_arr, capacity * sizeof(seq_t));
                if (!new_arr) exit(1);
            }
            new_arr[l++] = (seq_t)i;
        }
    }
    free(non_sqf);

    /* --- replace old array ---------------------------------------- */
    free(even_sqf);
    even_sqf = new_arr;
    even_len = l;
    even_cap = capacity;
    sieve_lim = limit;
}

/* ------------------------------------------------------------------ */
/* a:  return the n-th even squarefree number (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: not used in OEIS */
    /* ensure we have at least n terms */
    while (even_len < (size_t)n) {
        /* rough upper bound for the n-th term (empirical) */
        uint64_t need = ((uint64_t)n * 6ULL) + 10ULL;
        if (need <= sieve_lim) need = sieve_lim * 2ULL;   /* grow the sieve */
        recompute_sieve(need);
    }
    return even_sqf[(size_t)n - 1];
}
