/*
 * A215302  Primes congruent to {1, 2, 3, 4} mod 11.
 * https://oeis.org/A215302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 1, 2, 3 or 4 modulo 11
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 16, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is built around an incremental prime generator using
   trial division.  It works for both the C and C++ compiler kernels
   used by the test harness. */

#include <stdint.h>
#include <stdlib.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

/* ------------------------------------------------------------------ */
/*   Storage for the list of all primes found so far (used for
 *   trial division).                                            */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
#include <vector>
static std::vector<uint64_t> all_primes;
static std::vector<seq_t>   hit_primes;
static uint64_t             max_checked = 0;
#else
static uint64_t *all_primes = NULL;
static size_t    all_cnt    = 0;
static size_t    all_cap    = 0;
static seq_t  *hit_primes = NULL;
static size_t   hit_cnt   = 0;
static size_t   hit_cap   = 0;
static uint64_t max_checked = 0;
#endif

/* ----- C‑only helper functions ------------------------------------ */
#ifndef __cplusplus
static void append_uint64(uint64_t **arr, size_t *cnt, size_t *cap, uint64_t v) {
    if (*cap <= *cnt) {
        size_t newcap = (*cap ? (*cap << 1) : 16);
        *arr = (uint64_t*)realloc(*arr, newcap * sizeof *arr);
        *cap = newcap;
    }
    (*arr)[(*cnt)++] = v;
}
static void append_seq_t(seq_t **arr, size_t *cnt, size_t *cap, seq_t v) {
    if (*cap <= *cnt) {
        size_t newcap = (*cap ? (*cap << 1) : 16);
        *arr = (seq_t*)realloc(*arr, newcap * sizeof *arr);
        *cap = newcap;
    }
    (*arr)[(*cnt)++] = v;
}
#endif

/* ----- Core routine that guarantees at least n qualifying primes  ---- */
static void ensure_nth_hit(idx_t n) {
    if (n <= 0) return;                     /* n<1 is unused, but harmless */

#ifdef __cplusplus
    /* Initialise with the first prime 2. */
    if (all_primes.empty() || max_checked < 2) {
        all_primes.push_back(2);
        if ( (uint64_t)2 % 11 <= 4 && (uint64_t)2 % 11 >= 1 )
            hit_primes.push_back((seq_t)2);
        max_checked = 2;
    }
    while ( (idx_t)hit_primes.size() < n ) {
        ++max_checked;
        if (max_checked > 2 && (max_checked & 1ULL) == 0ULL) continue;  /* skip even */

        bool is_prime = true;
        for (size_t i = 0; i < all_primes.size(); ++i) {
            uint64_t p = all_primes[i];
            if (p * p > max_checked) break;
            if (max_checked % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            all_primes.push_back(max_checked);
            uint64_t r = max_checked % 11;
            if (r <= 4 && r >= 1)
                hit_primes.push_back((seq_t)max_checked);
        }
    }
#else
    /* C implementation using manual realloc. */
    if (all_cnt == 0 || max_checked < 2) {
        append_uint64(&all_primes, &all_cnt, &all_cap, 2);
        if ((uint64_t)2 % 11 <= 4 && (uint64_t)2 % 11 >= 1)
            append_seq_t(&hit_primes, &hit_cnt, &hit_cap, (seq_t)2);
        max_checked = 2;
    }
    while (hit_cnt < n) {
        ++max_checked;
        if (max_checked > 2 && (max_checked & 1ULL) == 0ULL) continue; /* skip even */

        bool is_prime = true;
        for (size_t i = 0; i < all_cnt; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > max_checked) break;
            if (max_checked % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            append_uint64(&all_primes, &all_cnt, &all_cap, max_checked);
            uint64_t r = max_checked % 11;
            if (r <= 4 && r >= 1)
                append_seq_t(&hit_primes, &hit_cnt, &hit_cap, (seq_t)max_checked);
        }
    }
#endif
}

/* ------------------------------------------------------------------ */
/*   Public sequence function                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, harmless */
    ensure_nth_hit(n);
#ifdef __cplusplus
    return hit_primes[n - 1];
#else
    return hit_primes[n - 1];
#endif
}
