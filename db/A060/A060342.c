/*
 * A060342  Smallest prime simultaneously the sum of two, three, ..., n
 *          consecutive composite numbers.
 * https://oeis.org/A060342
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the smallest prime that can be written as the sum of
 * k consecutive composite numbers for all k = 2..n.
 *  a(2)=17, a(3)=31, a(4)=311, a(5)=17783.
 * For n>5 no such prime exists (the sequence terminates). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* maximum composites we will consider – enough for all known terms */
#define MAX_COMPOSITES 300000

/* list of composites and cumulative sums of composites */
static size_t comps_cnt = 0;
static idx_t composites[MAX_COMPOSITES];
static seq_t prefix[MAX_COMPOSITES + 1];
static idx_t next_candidate = 4;     /* next integer to test for primality */

/* extend the composite list up to (and including) limit */
static void add_composites_up_to(idx_t limit)
{
    for (; next_candidate <= limit && comps_cnt < MAX_COMPOSITES; ++next_candidate) {
        if (!is_prime(next_candidate)) {
            composites[comps_cnt] = next_candidate;
            prefix[comps_cnt + 1] = prefix[comps_cnt] + (seq_t)next_candidate;
            ++comps_cnt;
        }
    }
}

/* check whether p can be expressed as a sum of k consecutive composites
 * for every k from 2 up to target_n */
static bool can_use_prime_p(idx_t p, idx_t target_n)
{
    for (idx_t k = 2; k <= target_n; ++k) {
        if (comps_cnt < (size_t)k) return false;   /* not enough composites yet */
        size_t max_start = comps_cnt - (size_t)k;
        bool found = false;
        for (size_t j = 0; j <= max_start; ++j) {
            seq_t sum = prefix[j + k] - prefix[j];
            if (sum == (seq_t)p) { found = true; break; }
            if (sum > (seq_t)p) break;   /* sums increase monotonically */
        }
        if (!found) return false;
    }
    return true;
}

/* find the smallest prime satisfying the condition for a given n (n>=2) */
static seq_t find_prime_for_n(idx_t target_n)
{
    idx_t candidate = 3;          /* start from the first prime */
    while (true) {
        if (!is_prime(candidate)) { ++candidate; continue; }

        /* make sure all composites up to the candidate are known */
        add_composites_up_to(candidate);

        if (can_use_prime_p(candidate, target_n))
            return (seq_t)candidate;

        ++candidate;
    }
}

/* main sequence function – memoises the few known terms */
seq_t a(idx_t n)
{
    /* offset 2: the sequence starts at a(2) */
    if (n < 2) return 0;

    /* the sequence ends at n = 5 – no further terms exist */
    if (n > 5) return 0;

    static bool initialized = false;
    static seq_t values[6] = {0};

    if (!initialized) {
        prefix[0] = 0;            /* initialise cumulative sum array */
        for (idx_t i = 2; i <= 5; ++i)
            values[(size_t)i] = find_prime_for_n(i);
        initialized = true;
    }

    return values[(size_t)n];
}
