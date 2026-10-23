/*
 * A035811  Coordination sequence for lattice D*_52 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{52} 2^k * C(52, k) * C(n-1, k-1) * + (n ≥ 26 ?
 *            2^52 * C(n+25, 51) : 0) * * First term: sum over k=1…52, where
 *            C(n-1, k-1) is taken as 0 if k-1>n-1. * Second term: present
 *            only for n≥26, because C(n+25, 51) is 0 otherwise. * * The
 *            function uses only integer arithmetic on seq_t type...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */
/* The following utility types are supplied by the build system:
   idx_t   – unsigned integer type for indices (uint64_t, unsigned, …)
   seq_t   – unsigned integer type for sequence values (uint8_t … unsigned __int128, or arbitrary‑precision) */

static bool initialized = false;
static seq_t pow2[53];
static seq_t binoms[53];

/* multiplicative binomial coefficient for 0 ≤ k ≤ 52.
 * If k > n or n < 0, returns 0.  The parameters are idx_t and hence unsigned,
 * so n < 0 is impossible; the function just checks k > n. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n || k > 52) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i); /* multiply first, then divide */
        res /= i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;         /* a(0) = 1 by definition */

    if (!initialized) {
        /* precompute powers of two up to 2^52 */
        pow2[0] = 1;
        for (idx_t i = 1; i <= 52; ++i)
            pow2[i] = pow2[i - 1] * 2;

        /* precompute C(52, k) for k = 0…52 */
        for (idx_t k = 0; k <= 52; ++k)
            binoms[k] = binom_small(52, k);

        initialized = true;
    }

    /* First part: Σ_{k=1}^{52} 2^k * C(52, k) * C(n-1, k-1) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= 52; ++k) {
        seq_t c = binom_small(n - 1, k - 1);   /* C(n-1, k-1), 0 when invalid */
        if (c == 0) continue;
        sum += pow2[k] * binoms[k] * c;
    }

    /* Second part: 2^52 * C(n+25, 51)  (only for n ≥ 26) */
    seq_t second = 0;
    if (n >= 26) {
        second = pow2[52] * binom_small(n + 25, 51);
    }

    return sum + second;
}
