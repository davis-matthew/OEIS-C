/*
 * A235711  Arithmetic derivative of quarter squares.
 * https://oeis.org/A235711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003415(A002620(n))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A002620(n) = floor(n*n/4)   (quarter squares) */
/*  A003415(m) = arithmetic derivative of m */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;          /* smallest prime factor array */
static idx_t spf_limit = 0;        /* maximum number for which spf is valid */

/* Ensure that spf array contains the smallest prime factors up to 'n' (inclusive) */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;

    /* Allocate a new array for spf of size (n+1) and build it */
    idx_t *new_spf = NULL;
    build_spf(n + 1, &new_spf);     /* expects length n+1 (indices 0..n) */
    free(spf);
    spf = new_spf;
    spf_limit = n;
}

/* Compute the arithmetic derivative of a number 'm' (m >= 0) using its prime factorization */
static seq_t arithmetic_derivative(idx_t m)
{
    if (m <= 1)
        return 0;

    ensure_spf(m);   /* the spf array is guaranteed to cover 'm' */
    seq_t d = 0;
    idx_t temp = m;
    seq_t orig = (seq_t)m;

    while (temp > 1)
    {
        idx_t p = spf[temp];
        if (p == 0)          /* safety: should not happen for temp > 1 */
            p = temp;

        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t term = (seq_t)exp * (orig / (seq_t)p);
        d += term;
    }

    return d;
}

/* Public function requested by the problem statement */
seq_t a(idx_t n)
{
    /* Compute quarter square: floor(n*n/4) */
    seq_t n_sq = (seq_t)n * ((seq_t)n);
    seq_t q = n_sq / 4;

    /* The arithmetic derivative of q */
    return arithmetic_derivative((idx_t)q);
}
