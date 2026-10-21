/*
 * A084049  Numbers k such that k and k+1 have the same sum of digits of
 *          divisors; A034690(k) = A034690(k+1).
 * https://oeis.org/A084049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 618, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A084049
    a(n) = the nth integer k such that the sum of the decimal digits of all
    divisors of k equals that of k+1.
    The sequence is obtained by a divisor‑sieve up to a fixed limit
    (enough to answer n ≤ 10000 quickly).  The sieve precomputes
    digit‑sum-of‑divisors for every number, then the list of matches
    is stored in a dynamic array of uint32_t (the values are small
    enough to fit easily).  The public function a(idx_t n) returns
    the n‑th match, converting the stored uint32_t to seq_t on the
    fly.  This method works for both C11 and C++17 builds with
    any seq_t / idx_t definition provided by the harness. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t); */

#define NMAX 2000000   /* sufficient for n up to about 10 000 */

static bool initialized = false;

/* Array of sum of digit sums of divisors; 1‑based, size NMAX+1 */
static uint32_t *sum_digits = NULL;

/* List of k with equal sums for k and k+1 (stored as 32‑bit indices) */
static uint32_t *matches = NULL;
static size_t match_len = 0, match_cap = 0;

/* ------------------------------------------------------------------ */
/*  Append a 32‑bit value to the matches array, reallocating if needed  */
/* ------------------------------------------------------------------ */
static void append_match(uint32_t val)
{
    if (match_len == match_cap) {
        size_t new_cap = match_cap ? (match_cap << 1) : 128;
        uint32_t *newp = (uint32_t*)realloc(matches, new_cap * sizeof(uint32_t));
        if (!newp) exit(EXIT_FAILURE);
        matches = newp;
        match_cap = new_cap;
    }
    matches[match_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  One‑time initialization: compute sum_digits[1..NMAX] and
    collect all matches.                                                    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate and zero the divisor‑sum array */
    sum_digits = (uint32_t*)calloc((size_t)(NMAX + 1), sizeof(uint32_t));
    if (!sum_digits) exit(EXIT_FAILURE);

    /* Sieve: for each divisor d add digit_sum(d) to all multiples */
    for (idx_t d = 1; d <= NMAX; ++d) {
        seq_t dsum = digit_sum((seq_t)d);
        uint32_t dsum_u = (uint32_t)dsum;          /* fits in uint32_t */
        for (idx_t m = d; m <= NMAX; m += d)
            sum_digits[(size_t)m] += dsum_u;
    }

    /* Collect matches k where sum_digits[k] == sum_digits[k+1] */
    for (idx_t k = 1; k < NMAX; ++k) {
        if (sum_digits[(size_t)k] == sum_digits[(size_t)(k + 1)])
            append_match((uint32_t)k);
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();                                 /* ensure data is ready */

    if (n == 0 || n > match_len)                /* out of range */
        return 0;  /* sentinel */

    return (seq_t)matches[(size_t)(n - 1)];     /* cast to seq_t on return */
}
