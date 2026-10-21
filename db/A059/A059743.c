/*
 * A059743  Number of points of norm^2 <= n^2 in the square lattice that are
 *          visible from the origin.
 * https://oeis.org/A059743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 185, u32 n <= 7152, u64 n <= 7152,
 *            u128 n <= 7152, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) in OEIS A059743.
   a(n) is the number of lattice points (i,j) with i²+j² ≤ n² that
   are visible from the origin (gcd(|i|,|j|)=1).  The formula used is

   a(n) = 1                                   // the origin
          + 4                               // (±1,0),(0,±1) for n ≥ 1
          + 4 · Σ_{i=1}^{n}
                #{ 1 ≤ j ≤ ⌊√(n²-i²)⌋ : gcd(i,j)=1 }

   The sum counts points in the first quadrant with i>0, j>0,
   multiplied by 4 for the four quadrants.  Only j>0 is considered
   because the axis points are handled separately.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Auxiliary integer square‑root for seq_t values                     */
/* ------------------------------------------------------------------ */
static inline seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;          /* hi is a safe upper bound */
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if ((uint64_t)mid * (uint64_t)mid <= (uint64_t)v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                     /* floor(sqrt(v)) */
}

/* ------------------------------------------------------------------ */
/* Smallest‑prime‑factor sieve up to the largest n encountered.       */
/* ------------------------------------------------------------------ */
static uint32_t *spf = NULL;
static uint32_t spf_max = 0;

static void ensure_spf(idx_t n)
{
    if ((uint32_t)n <= spf_max)
        return;
    uint32_t N = (uint32_t)n;
    uint32_t *new_spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!new_spf)
        exit(1);
    for (uint32_t i = 0; i <= N; ++i)
        new_spf[i] = i;
    if (N >= 1)
        new_spf[1] = 1;

    for (uint32_t i = 2; (uint64_t)i * i <= N; ++i)
        if (new_spf[i] == i)
            for (uint32_t j = i * i; j <= N; j += i)
                if (new_spf[j] == j)
                    new_spf[j] = i;

    if (spf != NULL)
        free(spf);
    spf = new_spf;
    spf_max = N;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;                        /* a(0) = 1 */

    ensure_spf(n);

    seq_t result = 1;                    /* the origin */

    /* The four points on the axes with coordinate 1 are visible.  */
    result += 4;                         /* valid for all n ≥ 1 */

    seq_t sum_coprime = 0;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (uint32_t i = 1; i <= (uint32_t)n; ++i) {
        seq_t rem = n_sq - (seq_t)i * (seq_t)i;
        seq_t m = isqrt_seq(rem);        /* max j (j>0) for this i */

        if (m == 0)
            continue;                    /* no positive j in this column */

        /* distinct prime divisors of i */
        uint32_t primes[16];
        int pk = 0;
        uint32_t tmp = i;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            primes[pk++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }

        seq_t local = m;                 /* start with all j's */
        int max_mask = 1 << pk;
        for (int mask = 1; mask < max_mask; ++mask) {
            seq_t div = 1;
            int bits = 0;
            for (int j = 0; j < pk; ++j) {
                if (mask & (1 << j)) {
                    div *= (seq_t)primes[j];
                    ++bits;
                }
            }
            seq_t term = m / div;
            if (bits & 1)          /* odd number of primes → subtract */
                local -= term;
            else                   /* even number → add */
                local += term;
        }

        sum_coprime += local;
    }

    result += 4 * sum_coprime;           /* 4 quadrants */
    return result;
}
