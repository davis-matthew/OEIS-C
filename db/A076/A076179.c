/*
 * A076179  2n-1 primes followed by 2n composite numbers.
 * https://oeis.org/A076179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076179: For each m ≥ 1 output (2m‑1) primes followed by
   (2m) composite (non‑prime) numbers.  The cumulative length up to
   block m is T(m) = 2m² + m. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Simple primality test by trial division (sufficient for the range
   necessary to generate the sequence up to n ≈ 10⁴). */
static bool isPrimeSimple(uint64_t v) {
    if (v < 2)                return false;
    if (v == 2 || v == 3)     return true;
    if (v % 2 == 0)           return false;
    for (uint64_t i = 3; i * i <= v; i += 2)
        if (v % i == 0)       return false;
    return true;
}

/* ---------- Storage for primes and composites ---------- */
#ifdef __cplusplus
static std::vector<seq_t> primes;
static std::vector<seq_t> composites;
static uint64_t prime_candidate    = 2;   /* next candidate to test for prime */
static uint64_t composite_candidate = 4;  /* next candidate to test for composite */
#else
static seq_t *primes    = NULL;
static size_t primes_size = 0, primes_cap = 0;
static seq_t *composites    = NULL;
static size_t composites_size = 0, composites_cap = 0;
static uint64_t prime_candidate    = 2;
static uint64_t composite_candidate = 4;
#endif

/* Ensure that at least needed primes are stored. */
static void ensure_primes_needed(size_t needed) {
#ifdef __cplusplus
    if (primes.size() < needed) {
        primes.reserve(needed);
        while (primes.size() < needed) {
            if (isPrimeSimple(prime_candidate))
                primes.push_back((seq_t)prime_candidate);
            ++prime_candidate;
        }
    }
#else
    if (primes_size >= needed) return;
    size_t new_cap = primes_cap ? primes_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    primes = tmp;
    primes_cap = new_cap;
    while (primes_size < needed) {
        if (isPrimeSimple(prime_candidate))
            primes[primes_size++] = (seq_t)prime_candidate;
        ++prime_candidate;
    }
#endif
}

/* Ensure that at least needed composites are stored. */
static void ensure_composites_needed(size_t needed) {
#ifdef __cplusplus
    if (composites.size() < needed) {
        composites.reserve(needed);
        while (composites.size() < needed) {
            if (!isPrimeSimple(composite_candidate))
                composites.push_back((seq_t)composite_candidate);
            ++composite_candidate;
        }
    }
#else
    if (composites_size >= needed) return;
    size_t new_cap = composites_cap ? composites_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    composites = tmp;
    composites_cap = new_cap;
    while (composites_size < needed) {
        if (!isPrimeSimple(composite_candidate))
            composites[composites_size++] = (seq_t)composite_candidate;
        ++composite_candidate;
    }
#endif
}

/* Cumulative length of sequence up to and including block m. */
static inline idx_t block_cumulative(idx_t m) {
    return 2 * m * m + m;
}

/* Binary search for the block containing position n. */
static inline idx_t find_block(idx_t n) {
    idx_t low = 1, high = 1;
    while (block_cumulative(high) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (block_cumulative(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset 0, harmless */
    idx_t m = find_block(n);                  /* block number (1‑based) */
    idx_t prev_len = block_cumulative(m - 1); /* length before this block */
    idx_t offset = n - prev_len - 1;          /* 0‑based position inside block */

    /* Block m: first (2m‑1) primes, then (2m) composites. */
    if (offset < (2 * m - 1)) {               /* prime part */
        idx_t prime_index = (m - 1) * (m - 1) + offset;
        ensure_primes_needed(prime_index + 1);
        return primes[prime_index];
    } else {                                  /* composite part */
        idx_t comp_rel  = offset - (2 * m - 1);
        idx_t comp_index = (m - 1) * m + comp_rel;
        ensure_composites_needed(comp_index + 1);
        return composites[comp_index];
    }
}
