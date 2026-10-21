/*
 * A227015  a(n) = floor(M(g(n-1)+1, ..., g(n))), where M = harmonic mean and
 *          g(n) = n^3 + n^2 + n + 1.
 * https://oeis.org/A227015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   harmonic_asym
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227015: a(n) = floor( (g(n)-g(n‑1)) / (H_{g(n)} - H_{g(n‑1)}) ),
   where g(n)=n^3+n^2+n+1 and H_m is the m‑th harmonic number.
   The implementation uses long double for high precision and
   converts the final integer result to seq_t via a 64‑bit
   intermediate, which is safe for all provided builds. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/harmonic_asym.h"

/* ----------  Harmonic number helpers  ---------- */

/*  For small m we compute H_m exactly by summation.  */
static long double harmonic_exact(idx_t m)
{
    long double sum = 0.0L;
    for (idx_t i = 1; i <= m; ++i)
        sum += 1.0L / (long double)i;
    return sum;
}

/*  Choose the best method according to magnitude.  */
static long double harmonic_value(idx_t m)
{
    /* A threshold where exact summation is still inexpensive. */
    const idx_t SMALL_LIMIT = 4000;
    return (m <= SMALL_LIMIT) ? harmonic_exact(m) : harmonic_asym(m);
}

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /*  Compute g(n) and g(n‑1) using 128‑bit arithmetic to avoid overflow.  */
    unsigned __int128 N    = (unsigned __int128)n;
    unsigned __int128 N2   = N * N;
    unsigned __int128 N3   = N2 * N;

    unsigned __int128 Nm1  = N - 1;
    unsigned __int128 Nm12  = Nm1 * Nm1;
    unsigned __int128 Nm13  = Nm12 * Nm1;

    idx_t g_nm1 = (idx_t)(Nm13 + Nm12 + Nm1 + 1);   /* g(n‑1) */
    idx_t g_n   = (idx_t)(N3 + N2 + N + 1);         /* g(n)   */

    /*  Number of terms in the interval [g(n‑1)+1 , g(n)]  */
    unsigned __int128 k128 = 3 * N2 - N + 1;
    uint64_t k = (uint64_t)k128;   /*   k <= 3*n^2 < 3*10^8 for n<=10^4   */

    /*  Harmonic numbers at the ends of the interval  */
    long double H_b = harmonic_value(g_n);
    long double H_a = harmonic_value(g_nm1);

    /*  Sum of reciprocals over the interval  */
    long double sum_inv = H_b - H_a;

    /*  Harmonic mean  */
    long double mean = (long double)k / sum_inv;

    /*  Convert to integer, guarding against floating‑point error  */
    uint64_t result = (uint64_t)floorl(mean + 1e-12L);

    return (seq_t)result;
}
