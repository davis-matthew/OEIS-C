/*
 * A225375  Odd numbers with exactly 2 distinct prime factors.
 * https://oeis.org/A225375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225375: odd numbers with exactly two distinct odd prime factors.
   The algorithm uses a smallest‑prime‑factor array for 64‑bit candidates
   and memoises the sequence values.  It works for any seq_t that the
   surrounding build supplies (integer types or arbitrary‑precision). */

/* build_spf provides: void build_spf(idx_t limit, idx_t **spf_out);
   seq_realloc provides: seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

/* static cache of computed terms */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of terms stored */
static size_t seq_cap  = 0;   /* current capacity of seq_cache */

/* static smallest‑prime‑factor array */
static idx_t *spf = NULL;
static idx_t  spf_limit = 0;

/* Ensure seq_cache has capacity for at least 'need' terms. */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* Ensure the SPF array covers at least up to 'limit'. */
static void ensure_spf_limit(idx_t limit)
{
    if (spf_limit >= limit) return;
    if (spf) free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Count distinct odd prime factors of x using the SPF array.
   Early exit if count exceeds 2. */
static unsigned count_distinct_odd_prime_factors(idx_t x)
{
    unsigned count = 0;
    idx_t prev = 0;
    while (x > 1) {
        idx_t p = spf[x];
        if (p != prev) {
            ++count;
            if (count > 2) return count;
            prev = p;
        }
        /* remove all copies of p */
        while (x % p == 0) x /= p;
    }
    return count;
}

/* Generate terms until we have at least n cached. */
static void generate_until(idx_t n)
{
    static idx_t next_candidate = 3;   /* start from the first odd composite */
    unsigned count;

    while (seq_len < (size_t)n) {
        /* Grow SPF if needed */
        if (next_candidate > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit << 1 : 4;
            while (next_candidate > new_limit) new_limit <<= 1;
            ensure_spf_limit(new_limit);
        }

        /* Only odd numbers are relevant. */
        if ((next_candidate & 1ULL) != 0) {
            count = count_distinct_odd_prime_factors(next_candidate);
            if (count == 2) {
                ensure_seq_capacity(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Public function required by OEIS. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 – safe fallback */
    if (seq_len < (size_t)n) {
        generate_until(n);
    }
    return seq_cache[(size_t)(n - 1)];
}
