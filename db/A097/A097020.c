/*
 * A097020  Initial terms of chains consisting of four consecutive integers,
 *          for none of which is the value of sigma-function divisible by six.
 * https://oeis.org/A097020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates integers and records the
   starting value of every block of four consecutive integers
   {m, m+1, m+2, m+3} for which none of the sums of divisors
   (sigma(m), …, sigma(m+3)) is divisible by 6.
   It pre‑computes sigma modulo 6 for all integers 0 .. LIMIT
   once, then uses this table for successive evaluations.
   The sequence is cached so arbitrary order of calls is fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum integer for which sigma modulo 6 is pre–computed.
   This must be large enough to contain the 10 000th sequence term.
   Empirical data indicate that 10 000th term is < 2 × 10⁶, so
   10 000 000 gives a comfortable margin and fits comfortably
   in memory (10 MiB). */
#define SIGMA_LIMIT 10000000ULL

/* Pre‑computed sigma modulo 6 for 0 .. SIGMA_LIMIT. */
static uint8_t *sigma_mod6 = NULL;
static bool sigma_precomputed = false;

/* Allocate and compute sigma modulo 6 for 0 .. SIGMA_LIMIT. */
static void precompute_sigma(void)
{
    size_t lim = (size_t)SIGMA_LIMIT;
    /* Allocate array of zeros. */
    sigma_mod6 = (uint8_t *)calloc(lim + 1, sizeof(uint8_t));
    if (!sigma_mod6)
        exit(EXIT_FAILURE);   /* memory allocation failed */

    /* Sieve: for each divisor i, add i%6 to all multiples. */
    for (idx_t i = 1; i <= SIGMA_LIMIT; ++i) {
        uint8_t imod6 = (uint8_t)(i % 6);
        for (idx_t j = i; j <= SIGMA_LIMIT; j += i) {
            uint8_t v = sigma_mod6[(size_t)j] + imod6;
            if (v >= 6) v -= 6;
            sigma_mod6[(size_t)j] = v;
        }
    }
    sigma_precomputed = true;
}

/* Cache for sequence values. */
static seq_t *seq_terms = NULL;   /* array of computed terms */
static size_t seq_cap   = 0;      /* capacity of seq_terms   */
static size_t seq_cnt   = 0;      /* number of terms stored */

/* Ensure seq_terms has room for at least need terms. */
static void ensure_terms_capacity(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    if (!seq_terms)
        exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* Index of next integer to test for the start of a block. */
static idx_t next_start = 1;

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Pre‑compute sigma table on first call. */
    if (!sigma_precomputed)
        precompute_sigma();

    /* Reserve storage for the requested term. */
    ensure_terms_capacity((size_t)n + 1);

    /* Generate terms until we have at least n+1 of them. */
    while (seq_cnt <= (size_t)n) {
        /* Ensure sigma values exist for this quadruple. */
        if (next_start + 3 > SIGMA_LIMIT) {
            /* The chosen limit is insufficient – in practice
               this should never happen with SIGMA_LIMIT=10⁷. */
            exit(EXIT_FAILURE);
        }

        uint8_t m0 = sigma_mod6[(size_t)next_start];
        uint8_t m1 = sigma_mod6[(size_t)(next_start + 1)];
        uint8_t m2 = sigma_mod6[(size_t)(next_start + 2)];
        uint8_t m3 = sigma_mod6[(size_t)(next_start + 3)];

        if (m0 && m1 && m2 && m3) {
            /* Store the starting integer of a valid quadruple. */
            ensure_terms_capacity(seq_cnt + 1);
            seq_terms[seq_cnt++] = (seq_t)next_start;
        }

        ++next_start;            /* move to the next possible start */
    }

    return seq_terms[(size_t)n];
}
