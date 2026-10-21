/*
 * A060431  Number of cubefree numbers <= n.
 * https://oeis.org/A060431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d>=1} mu(d) * floor(n / d^3)
 * Range:     u8 n <= 305, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where mu(d) is the Möbius function. */

#include <stdint.h>
#include <stdbool.h>

/* --- Möbius tables (pre‑computed up to a fixed limit) --- */
#define MU_MAX 3000000U   /* sufficient for cube root of any 64‐bit n */

/*  static arrays are zero‑initialised by the compiler */
static int   mu[MU_MAX + 1];
static bool  composite[MU_MAX + 1];
static int   primes[MU_MAX];
static int   prime_cnt = 0;
static bool  mu_precomputed = false;

/* Initialise mu[1..MU_MAX] by a linear sieve */
static void
init_mu(void)
{
    if (mu_precomputed)
        return;

    mu[1] = 1;
    for (idx_t i = 2; i <= MU_MAX; ++i) {
        if (!composite[(int)i]) {
            primes[prime_cnt++] = (int)i;
            mu[(int)i]          = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            idx_t p  = (idx_t)primes[j];
            idx_t ip = i * p;
            if (ip > MU_MAX)
                break;
            composite[(int)ip] = true;
            if (i % p == 0) {
                mu[(int)ip] = 0;
                break;
            } else {
                mu[(int)ip] = -mu[(int)i];
            }
        }
    }
    mu_precomputed = true;
}

/* Integer cube root: floor of cbrt(n) */
static idx_t
cube_root_floor(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* find an upper bound for cube root (without overflow) */
    while (true) {
        __int128 prod = (__int128)hi * hi * hi;
        if (prod > n)
            break;
        hi <<= 1;
        if (hi == 0) {        /* overflow in shifting: use MU_MAX as cap */
            hi = MU_MAX;
            break;
        }
    }
    /* binary search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 prod = (__int128)mid * mid * mid;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --- main sequence function --- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* behaviour below offset 1 does not matter */

    init_mu();

    idx_t max_d = cube_root_floor(n);
    if (max_d > MU_MAX)
        return 0;          /* safety: should not happen for 64‑bit n */

    __int128 sum = 0;      /* signed intermediate sum */

    for (idx_t d = 1; d <= max_d; ++d) {
        int mu_d = mu[(int)d];
        if (mu_d == 0)
            continue;

        /* compute d^3 safely */
        idx_t cube = (idx_t)(((__int128)d * d) * d);   /* d^3 <= n */
        idx_t term = n / cube;                         /* floor division */

        if (mu_d > 0)
            sum += term;
        else
            sum -= term;
    }

    /* sum is guaranteed non‑negative for n >= 1 */
    return (seq_t)sum;
}
