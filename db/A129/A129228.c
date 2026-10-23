/*
 * A129228  a(n) is that prime number p less than n*Pi such that n*Pi/p has
 *          the largest fractional part.
 * https://oeis.org/A129228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = the largest fractional part of (n*π)/p for all primes p<n*π.
   For each n≥1, compute n⋅π as a double, then test all primes p<p<floor(n⋅π),
   selecting the one that maximizes fmod(n⋅π,p)/p.  The primes are pre‑computed once
   up to 40000 (more than 10000⋅π) using the supplied utility.  This routine
   works for the 64‑bit, 128‑bit, and arbitrary‑precision builds; the double
   computation is used only to compare fractional parts, which is sufficient
   for n up to 10 000 as required. */

#include <float.h>
#include <math.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* static storage for primes */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_ready = false;

/* initialize primes once */
static void init_primes(void)
{
    if (!primes_ready) {
        /* 40000 exceeds 10000*π (~31416) */
        build_primes_upto(40000, (uint64_t **)&primes, &primes_cnt);
        primes_ready = true;
    }
}

/* constant π to double precision */
static const double PI = 3.1415926535897932384626433832795028841971693993751;

/* main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range, irrelevant */

    init_primes();

    /* compute n⋅π and bound floor(n⋅π) - 1 to ensure p < n⋅π */
    double npi = (double)n * PI;
    uint64_t bound = (uint64_t)(npi - 1e-12);   /* strict inequality */

    double best_frac = -1.0;
    seq_t best_p = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if ((uint64_t)p >= bound) break;

        double rem = fmod(npi, (double)p);
        double frac = rem / (double)p;
        if (frac > best_frac) {
            best_frac = frac;
            best_p = (seq_t)p;
        }
    }

    return best_p;
}
