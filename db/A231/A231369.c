/*
 * A231369  Numbers n such that antisigma(x) = n has no solution, where
 *          antisigma(m) = A024816(m) = the sum of the nondivisors of m that
 *          are between 1 and m.
 * https://oeis.org/A231369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A231369 – numbers n such that antisigma(x) = n has no solution.
    A(n) is the nth positive integer that is NOT in the set { A024816(m) : m ≥ 1 }.
    A024816(m) = sum of the non‑divisors of m between 1 and m
                = m·(m+1)/2 – σ(m), where σ(m) is the sum of divisors of m.
    We generate antisigma values in increasing order (they are non‑decreasing)
    and keep a bitset of all seen antisigma values.
    For every missing integer we store it in an array; no hard‑coded table
    is used.  This algorithm works for any seq_t and for any number of
    requested terms.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sigma_of.h"

static uint64_t *seen         = NULL;   /* bitset for seen antisigma values */
static size_t   seen_cap     = 0;        /* capacity in uint64_t words */

static seq_t   *missing_arr  = NULL;   /* array of missing numbers */
static size_t   missing_cap  = 0;
static size_t   missing_len  = 0;

static size_t   next_check   = 1;      /* smallest integer not yet decided */
static idx_t    processed_m  = 0;      /* largest m processed */

/* Ensure the bitset can hold `val`.  Grow in powers of two. */
static void ensure_seen_capacity(idx_t val)
{
    idx_t needed_words = (val >> 6) + 1;
    if ((size_t)needed_words > seen_cap) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 16;
        while (new_cap < (size_t)needed_words) new_cap <<= 1;
        uint64_t *new_seen = (uint64_t *)realloc(seen, new_cap * sizeof(uint64_t));
        if (!new_seen) exit(1);                 /* fail fast */
        memset(new_seen + seen_cap, 0, (new_cap - seen_cap) * sizeof(uint64_t));
        seen = new_seen;
        seen_cap = new_cap;
    }
}

/* Grow the missing‑numbers array when needed. */
static void grow_missing(void)
{
    if (missing_cap == 0) {
        missing_cap = 16;
        missing_arr = (seq_t *)malloc(missing_cap * sizeof(seq_t));
    } else {
        missing_cap <<= 1;
        missing_arr = (seq_t *)realloc(missing_arr, missing_cap * sizeof(seq_t));
    }
    if (!missing_arr) exit(1);
}

/* Make sure that at least `target` missing numbers are known. */
static void ensure_missing(idx_t target)
{
    while (missing_len < target) {
        /* Process further m until we have reached an antisigma value
           greater than the current `next_check`.  Then `next_check`
           is guaranteed to have a final status. */
        while (1) {
            uint64_t new_m = processed_m + 1;
            seq_t m_seq = (seq_t)new_m;
            seq_t total = m_seq * (m_seq + 1) / 2;
            seq_t sigma = sigma_of(new_m);
            seq_t antisigma = total - sigma;

            idx_t antis_idx = (idx_t)antisigma;   /* fits for the needed range */
            ensure_seen_capacity(antis_idx);
            uint64_t *w = seen + (antis_idx >> 6);
            *w |= (1ULL << (antis_idx & 63));

            processed_m = new_m;

            if (antisigma > next_check) break;   /* all values ≤ next_check processed */
        }

        /* Decide the status of `next_check`. */
        if (!(seen[(next_check >> 6)] & (1ULL << (next_check & 63)))) {
            if (missing_len == missing_cap) grow_missing();
            missing_arr[missing_len++] = (seq_t)next_check;
        }
        next_check++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – undefined for 0 */
    ensure_missing(n);
    return missing_arr[n - 1];
}
