/*
 * A106816  Smallest prime of the set of five consecutive primes whose sum of
 *          digits is a set of five distinct primes.
 * https://oeis.org/A106816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest prime in a block of five consecutive primes
 *            for which the sums of digits of those primes are five distinct
 *            primes. The function is computed by a simple incremental
 *            primality test and sliding‑window scan
 * Range:     u8 none, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generator for OEIS A106816. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/is_prime.h"

/* --- small utilities ---------------------------------------------------- */

static inline unsigned int digit_sum_uint64(uint64_t x)
{
    unsigned int s = 0;
    while (x) {
        s += (unsigned int)(x % 10);
        x /= 10;
    }
    return s;
}

/* --- window test ------------------------------------------------------- */

static inline bool window_good(const uint64_t w[5])
{
    unsigned int sums[5];
    for (int i = 0; i < 5; ++i)
        sums[i] = digit_sum_uint64(w[i]);

    /* all sums must be prime */
    for (int i = 0; i < 5; ++i)
        if (!is_prime((uint64_t)sums[i]))
            return false;

    /* sums must be pairwise distinct */
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            if (sums[i] == sums[j])
                return false;

    return true;
}

/* --- dynamic array for sequence values ------------------------------- */

static seq_t *seq_ans = NULL;     /* array of found terms */
static size_t seq_len = 0;        /* number of stored terms     */
static size_t seq_cap = 0;        /* allocated capacity         */

static void ensure_seq_cap(size_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap < 8 ? 8 : seq_cap * 2;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_ans = (seq_t *)realloc(seq_ans, sizeof(seq_t) * new_cap);
    seq_cap = new_cap;
}

/* --- sliding window of the last five primes -------------------------- */

static uint64_t last_primes[5];
static int last_count = 0;        /* number of primes currently in window */

/* add a prime to the window and, if it forms a good block,
   store the first prime of that block into seq_ans. */
static void handle_prime(uint64_t p)
{
    if (last_count < 5) {
        last_primes[last_count++] = p;
    } else {
        for (int i = 0; i < 4; ++i)
            last_primes[i] = last_primes[i + 1];
        last_primes[4] = p;
    }

    if (last_count == 5 && window_good(last_primes)) {
        ensure_seq_cap(seq_len + 1);
        seq_ans[seq_len++] = (seq_t)last_primes[0];
    }
}

/* --- prime generator --------------------------------------------------- */

static uint64_t next_candidate = 2;   /* next number to test for primality */
static bool global_initialized = false;

/* generate enough terms so that seq_len >= n */
static void generate_until(idx_t n)
{
    if (!global_initialized) {
        /* prime 2 is special */
        handle_prime(2);
        next_candidate = 3;
        global_initialized = true;
    }

    while (seq_len < (size_t)n) {
        /* find the next prime */
        if ((next_candidate & 1) == 0)
            next_candidate++;          /* skip even numbers */

        while (!is_prime(next_candidate))
            next_candidate += 2;       /* only test odd numbers */

        /* next_candidate is prime */
        handle_prime(next_candidate);
        next_candidate += 2;           /* advance to next odd candidate */
    }
}

/* --- public function --------------------------------------------------- */

seq_t a(idx_t n)
{
    generate_until(n);
    return seq_ans[n - 1];
}
