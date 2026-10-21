/*
 * A085347  Least number x so that number of primes not exceeding sigma(x)
 *          equals n.
 * https://oeis.org/A085347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal x such that pi(sigma(x)) = n. pi = prime-counting
 *            function, sigma = sum of divisors
 * Range:     u8 n <= 54, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   compute_pi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/compute_pi.h"   /* seq_t compute_pi(idx_t n); */
#include "../utils/sigma_of.h"    /* seq_t sigma_of(idx_t n); */

/* Global state used for memoisation.  All indices are 1‑based.  */
static seq_t *minX   = NULL;   /* minX[p] = minimal x with pi(sigma(x)) == p */
static idx_t  capX   = 0;      /* allocated capacity of minX */
static idx_t  maxX   = 0;      /* largest x that has been processed */

/* Ensure minX has space for at least 'needed' indices.  */
static void ensure_minX_capacity(idx_t needed)
{
    if (capX >= needed) return;

    idx_t newCap = capX ? capX << 1 : 1;
    while (newCap < needed) newCap <<= 1;

    seq_t *tmp = (seq_t *)realloc(minX, newCap * sizeof(seq_t));
    if (!tmp) exit(1);                  /* out of memory */

    /* Zero the new part so that an unset element stays (seq_t)0. */
    if (newCap > capX)
        memset(tmp + capX, 0, (newCap - capX) * sizeof(seq_t));

    minX = tmp;
    capX = newCap;
}

/* Least number x with exactly n primes not exceeding sigma(x). */
seq_t a(idx_t n)
{
    /* Definition for n == 1.  */
    if (n == 1U) return 0;

    /* Make sure storage exists for the requested index. */
    ensure_minX_capacity(n + 1);
    if (minX[n] != 0)                 /* already found earlier */
        return minX[n];

    /* Search x incrementally until we hit the desired pi(sigma(x)). */
    for (idx_t x = maxX + 1; ; ++x)
    {
        /* sigma(x) fits into idx_t for the sizes we will handle. */
        seq_t sigma  = sigma_of(x);
        idx_t pi_val = (idx_t)compute_pi((idx_t)sigma);

        /* Record the first x that yields this pi value. */
        ensure_minX_capacity(pi_val + 1);
        if (minX[pi_val] == 0)
            minX[pi_val] = (seq_t)x;

        if (pi_val == n)           /* desired value reached */
            return minX[n];

        maxX = x;                  /* extend processed range */
    }
}
