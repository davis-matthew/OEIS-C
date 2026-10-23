/*
 * A178755  Integers that become semiprime when any single digit is removed.
 * https://oeis.org/A178755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Sequence of integers that become semiprime when any single digit
 *            is removed
 * Range:     u8 n <= 18, u16 n <= 585, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation monitors the sequence by dynamically generating candidates and
   testing each one: for every removal of a single decimal digit the remaining
   number must be a semiprime (product of exactly two primes, possibly equal). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/ensure_capacity.h"

/* --------------------------------------------------------------------------- */
/*  Helper: test whether a (small) integer is a semiprime.                      */
/* --------------------------------------------------------------------------- */
static bool is_semiprime(seq_t num)
{
    /* We only handle values that fit in 64 bits; for larger values we return
       false, which will simply exclude very large candidates from the
       sequence.  The typical terms of A178755 all fit in 64 bits for the
       requested range. */
    if ((seq_t)(uint64_t)num != num)
        return false;

    uint64_t v = (uint64_t)num;
    if (v < 4)               /* 0,1,2,3 cannot be semiprime */
        return false;

    size_t cnt = 0;
    uint64_t primes[32];
    uint64_t exps[32];

    factor_u64(v, primes, exps, &cnt);

    uint64_t total_exp = 0;
    for (size_t i = 0; i < cnt; ++i)
        total_exp += exps[i];

    return total_exp == 2;
}

/* --------------------------------------------------------------------------- */
/*  Helper: test whether an integer x satisfies the defining property.        */
/* --------------------------------------------------------------------------- */
static bool is_good(seq_t x)
{
    /* Extract decimal digits of x into an array MSB→LSB. */
    seq_t tmp = x;
    int digits[32];
    int dcnt = 0;

    if (tmp == 0) {          /* single zero digit */
        digits[dcnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[dcnt++] = (int)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to MSB first. */
        for (int i = 0; i < dcnt / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[dcnt - 1 - i];
            digits[dcnt - 1 - i] = t;
        }
    }

    /* For each position, form the number after removing that digit. */
    for (int i = 0; i < dcnt; ++i) {
        seq_t rem = 0;
        for (int j = 0; j < dcnt; ++j) {
            if (j == i) continue;
            rem = rem * 10 + digits[j];
        }
        if (!is_semiprime(rem))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------------- */
/*  Main sequence routine.                                                    */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Cache previously computed terms. */
    static seq_t *seq      = NULL;
    static size_t   cap   = 0;
    static idx_t    len   = 0;   /* number of terms already stored */

    if (n == 0)                /* the OEIS offset is 1, so n≥1 is needed */
        return 0;

    /* Return cached value if already computed. */
    if (n <= len)
        return seq[n - 1];

    /* Determine the starting candidate: after the last cached term. */
    seq_t cand = 44;    /* first term in the sequence */
    if (len > 0)
        cand = seq[len - 1] + 1;

    /* Ensure the cache has space for all requested terms. */
    ensure_capacity((size_t)n, &cap, (void **)&seq, sizeof(seq_t));

    /* Generate terms until we have the n-th one. */
    for (; len < n; ++cand) {
        if (is_good(cand))
            seq[len++] = cand;
    }

    return seq[n - 1];
}
