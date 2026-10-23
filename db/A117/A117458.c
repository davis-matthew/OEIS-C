/*
 * A117458  Primes and their indices such that when their respective SOD's are
 *          both prime, the SOD of the index is the nextprime of the prime
 *          SOD.
 * https://oeis.org/A117458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117458: Primes p whose index i has
   digit_sum(p) and digit_sum(i) prime, and digit_sum(i)
   is the next prime after digit_sum(p).
   a(n) = the n-th such prime (offset 1). */
/* The routine uses a simple incremental prime generator and
   memoises the results.  It depends on the utility functions
   digit_sum(seq_t) and is_prime_small(seq_t).  For next prime
   after a small integer it performs a simple increment-and-test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

/* Small helper: next prime after a given small integer n. */
static int next_prime_int(int n)
{
    int cand = n + 1;
    while (1)
    {
        if (is_prime_small((seq_t)cand))
            return cand;
        cand++;
    }
}

/* Memoised storage for the sequence values. */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* State for incremental prime generation. */
static uint64_t prime_counter    = 0;   /* number of primes seen so far (1‑based) */
static uint64_t candidate_value = 2;    /* next integer to test for primality */

/* Ensure that the sequence contains at least `need` entries. */
static void build_sequence(idx_t need)
{
    while (seq_len < (size_t)need)
    {
        /* Test whether candidate_value is a prime. */
        if (is_prime_small((seq_t)candidate_value))
        {
            prime_counter++;  /* its index (1‑based) */

            /* Compute digit sums of the prime and its index. */
            int dsp = (int)digit_sum((seq_t)candidate_value);
            int sdi = (int)digit_sum((seq_t)prime_counter);

            /* Check the prime‑sum conditions. */
            if (is_prime_small((seq_t)dsp) &&
                is_prime_small((seq_t)sdi) &&
                sdi == next_prime_int(dsp))
            {
                /* Append to memoised array. */
                if (seq_len == seq_cap)
                {
                    seq_cap = seq_cap ? seq_cap * 2 : 64;
                    seq_vals = (seq_t *)realloc(seq_vals,
                                                seq_cap * sizeof(seq_t));
                }
                seq_vals[seq_len++] = (seq_t)candidate_value;
            }
        }
        candidate_value++;  /* move to next integer for primality testing */
    }
}

/* Public function required by the evaluator. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; n should be ≥1 in practice */

    /* Compute more terms if necessary. */
    build_sequence(n);

    return seq_vals[n - 1];
}
