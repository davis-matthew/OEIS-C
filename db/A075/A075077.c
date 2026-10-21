/*
 * A075077  a(1) = 1 and then the smallest number not occurring earlier such
 *          that a(n) + a(n+1) gives a different composite number (not
 *          obtained earlier).
 * https://oeis.org/A075077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_bool_capacity, mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A075077:
 * a(1) = 1.
 * For n > 1, a(n) is the smallest unused integer such that
 * a(n-1) + a(n) is a composite number not seen before.
 * The implementation keeps track of already used values and sums
 * and searches for the next term when needed.  It works for any
 * seq_t and idx_t provided by the build.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_bool_capacity.h"
#include "../utils/mod_mul_uint64.h"

/* ------------------------------------------------------------------
 * Primality test for 64‑bit integers – used to check that a sum is
 * composite.  This routine works regardless of build mode because
 * sums will always be small enough to fit into uint64_t for the
 * lengths needed in practice.
 * ------------------------------------------------------------------ */

static uint64_t mod_pow_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d)
    {
        if (d & 1) result = mod_mul_uint64(result, a, mod);
        a = mod_mul_uint64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n % small[i] == 0) return n == small[i];

    static const uint64_t bases[] = {2,325,9375,28178,450775,
                                     9780504,1795265022};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------
 * State for memoised computation
 * ------------------------------------------------------------------ */
static seq_t *seq = NULL;          /* computed terms */
static size_t seq_len = 0;         /* number of stored terms */
static size_t seq_cap = 0;         /* current capacity of seq */

static bool *seen_vals = NULL;     /* values already used */
static size_t seen_cap = 0;

static bool *seen_sums = NULL;     /* sums already seen */
static size_t seen_sum_cap = 0;

/* ------------------------------------------------------------------
 * Public routine
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* harmless fallback */

    /* initialise a(1) = 1 if this is the first call */
    if (seq_len == 0)
    {
        if (seq_cap == 0)
        {
            seq_cap = 1;
            seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
            if (!seq) abort();
        }
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_bool_capacity(&seen_vals, &seen_cap, 1);
        seen_vals[1] = true;
    }

    /* extend the sequence up to index n */
    while (seq_len < (size_t)n)
    {
        seq_t prev = seq[seq_len - 1];
        uint64_t cand = 1;

        /* search for the next term */
        while (true)
        {
            /* skip numbers already taken */
            ensure_bool_capacity(&seen_vals, &seen_cap, cand);
            if (seen_vals[(size_t)cand]) { ++cand; continue; }

            seq_t candV = (seq_t)cand;
            seq_t sumV  = prev + candV;
            uint64_t sum = (uint64_t)sumV;       /* sum will be small */

            /* require composite sum */
            if (is_prime_uint64(sum)) { ++cand; continue; }

            /* sum must be new */
            ensure_bool_capacity(&seen_sums, &seen_sum_cap, sum);
            if (seen_sums[(size_t)sum]) { ++cand; continue; }

            /* accept the candidate */
            if (seq_len == seq_cap)
            {
                seq_cap = (seq_cap > 0) ? seq_cap << 1 : 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) abort();
            }
            seq[seq_len++] = candV;

            seen_vals[(size_t)cand] = true;
            seen_sums[(size_t)sum] = true;
            break;
        }
    }

    return seq[(size_t)n - 1];
}
