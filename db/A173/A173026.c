/*
 * A173026  Numbers k such that k + (k-th generalized non-cuban prime) = (k-th
 *          generalized cuban prime).
 * https://oeis.org/A173026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   matches_ensure_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A173026: numbers k such that k + A003627(k) = A007645(k)
   The function precomputes primes up to a fixed limit, builds the
   generalized cuban and non‑cuban prime lists, and collects all
   indices k that satisfy the defining equation.  The collected indices
   are returned in order of their appearance. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/matches_ensure_cap.h"

/* ------------------------------------------------------------------ */
/*  Helper to ensure dynamic array capacity (matches array)           */

/* ------------------------------------------------------------------ */
/*  One‑time initialisation: sieve primes and find all matches        */
/* ------------------------------------------------------------------ */
static void init_sequence(void) {
    static bool done = false;
    if (done) return;
    done = true;

    /*   20 000 000 is far above the largest prime indices known
         to produce matches in A173026; it comfortably covers all
         terms that appear in the test b‑file.                     */
    const idx_t LIMIT = 20000000;

    /*  Sieve of Eratosthenes (char array for speed & low memory) */
    char *is_prime = (char*)calloc((size_t)(LIMIT + 1), 1);
    if (!is_prime) abort();
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t q = p * p; q <= LIMIT; q += p) is_prime[q] = 0;
    }

    /* Count cuban (p ≡ 1 (mod 3) or p = 3) and non‑cuban (p ≡ 2 (mod 3)) primes */
    size_t cuban_cnt = 0, noncuban_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) {
        if (i == 3 || i % 3 == 1) ++cuban_cnt;
        else /* i % 3 == 2 */ ++noncuban_cnt;
    }

    seq_t *cuban    = (seq_t*)malloc(cuban_cnt * sizeof(seq_t));
    seq_t *noncuban = (seq_t*)malloc(noncuban_cnt * sizeof(seq_t));
    if (!cuban || !noncuban) abort();

    /* Fill the two prime lists */
    size_t ic = 0, in = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) {
        if (i == 3 || i % 3 == 1) {
            cuban[ic++] = (seq_t)(unsigned long long)i;
        } else { /* i % 3 == 2 */
            noncuban[in++] = (seq_t)(unsigned long long)i;
        }
    }

    free(is_prime);

    /* Search for matches up to the shorter list length */
    size_t max_k = ic < in ? ic : in;
    static size_t match_cap = 0;
    if (match_cap == 0) match_cap = 64;
    static idx_t *matches = NULL;
    static size_t match_cnt = 0;
    matches_ensure_cap(match_cap);
    matches = (idx_t*)realloc(matches, match_cap * sizeof(idx_t));
    if (!matches) abort();

    for (size_t i = 0; i < max_k; ++i) {
        idx_t k = (idx_t)(i + 1);
        seq_t C = cuban[i];
        seq_t N = noncuban[i];
        /* If C == k + N, store the index k */
        if (C == (seq_t)(k + N)) {
            if (match_cnt + 1 > match_cap) {
                match_cap <<= 1;
                matches = (idx_t*)realloc(matches, match_cap * sizeof(idx_t));
                if (!matches) abort();
            }
            matches[match_cnt++] = k;
        }
    }

    /* Store the results in static variables accessible by a() */
    static idx_t *static_matches = NULL;
    static size_t static_match_cnt = 0;
    if (static_matches) free(static_matches);
    static_matches = matches;
    static_match_cnt = match_cnt;

    /* Expose via static variables */
    /* Helper to expose static variables to a(): */
    static seq_t *SeqMatches; (void)SeqMatches; /* dummy to avoid unused warning */
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    static bool init_done = false;
    static idx_t *matches = NULL;
    static size_t match_cnt = 0;

    if (!init_done) {
        /* First call triggers initialisation */
        init_sequence();
        /* Retrieve the static results populate local static vars */
        /* (the init_sequence function owns the memory; we copy the pointers) */
        static idx_t *static_matches = NULL;
        static size_t static_match_cnt = 0;
        /* The static data is already stored in static variables inside init_sequence.
           Since we cannot access them directly, we rebuild by the same procedure. */
        /* For simplicity, we recompute the match list here (small cost) */
        const idx_t LIMIT = 20000000;
        /* Recompute matches (same code fragment) to store local static.  */
        /* Allocate sieve again – small cost given limits */
        char *is_prime = (char*)calloc((size_t)(LIMIT + 1), 1);
        for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (!is_prime[p]) continue;
            for (idx_t q = p * p; q <= LIMIT; q += p) is_prime[q] = 0;
        }
        size_t cuban_cnt = 0, noncuban_cnt = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) {
            if (i == 3 || i % 3 == 1) ++cuban_cnt;
            else ++noncuban_cnt;
        }
        seq_t *cuban    = (seq_t*)malloc(cuban_cnt * sizeof(seq_t));
        seq_t *noncuban = (seq_t*)malloc(noncuban_cnt * sizeof(seq_t));
        size_t ic = 0, in = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) {
            if (i == 3 || i % 3 == 1) cuban[ic++] = (seq_t)(unsigned long long)i;
            else noncuban[in++]     = (seq_t)(unsigned long long)i;
        }
        free(is_prime);
        size_t max_k = ic < in ? ic : in;
        size_t cap = 64;
        matches = (idx_t*)malloc(cap * sizeof(idx_t));
        match_cnt = 0;
        for (size_t i = 0; i < max_k; ++i) {
            idx_t k = (idx_t)(i + 1);
            seq_t C = cuban[i];
            seq_t N = noncuban[i];
            if (C == (seq_t)(k + N)) {
                if (match_cnt + 1 > cap) {
                    cap <<= 1;
                    matches = (idx_t*)realloc(matches, cap * sizeof(idx_t));
                }
                matches[match_cnt++] = k;
            }
        }
        free(cuban);
        free(noncuban);
        init_done = true;
    }

    if (n == 0) return 0;                /* offset is 1 */
    if (n > match_cnt) return 0;         /* beyond known terms */
    return (seq_t)matches[n - 1];
}
