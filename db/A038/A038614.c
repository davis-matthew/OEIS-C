/*
 * A038614  Primes not containing the digit '6'.
 * https://oeis.org/A038614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth prime that does not contain the digit '6'. The
 *            function generates primes by incremental trial division and
 *            tests each for the presence of the digit 6, storing the
 *            qualifying primes in a second dynamic array. It memoises both
 *            lists so that calls in arbitrary order are fast
 * Range:     u8 n <= 50, u16 n <= 4423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_digit_6.h"

/* Grow a dynamic array, ensuring it can hold 'needed' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(**arr));
        *cap = new_cap;
    }
}

/* Storage for all primes found so far and for the filtered primes */
static seq_t *primes = NULL;
static size_t primes_cap   = 0;
static size_t primes_cnt   = 0;

static seq_t *good   = NULL;
static size_t good_cap   = 0;
static size_t good_cnt   = 0;

/* Next odd number to test for primality */
static idx_t next_candidate = 3;

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no defined value at offset 0 */

    /* Initialise on the first call */
    if (primes_cnt == 0) {
        grow_array(&primes, &primes_cap, 1);
        primes[primes_cnt++] = 2;

        grow_array(&good, &good_cap, 1);
        good[good_cnt++] = 2;
    }

    /* Keep generating primes until we have n qualifying primes */
    while (good_cnt < n) {
        seq_t cand = next_candidate;
        next_candidate += 2;          /* skip even numbers */

        /* Trial division using previously found primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            if ((p * p) > cand) break;
            if ((cand % p) == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            grow_array(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = cand;

            if (!has_digit_6(cand)) {
                grow_array(&good, &good_cap, good_cnt + 1);
                good[good_cnt++] = cand;
            }
        }
    }

    /* good is zero‑based, n is 1‑based */
    return good[n - 1];
}
