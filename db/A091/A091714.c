/*
 * A091714  Row sums of triangle A052313, which is the matrix square of the
 *          triangle of circular binomial coefficients (A047996).
 * https://oeis.org/A091714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} N(n,k) * B(k) where N(n,k) =
 *            (1/n)*Σ_{d|gcd(n,k)} φ(d)*binom(n/d,k/d) and B(k) =
 *            (1/k)*Σ_{e|k} φ(e)*2^{k/e}
 * Range:     u8 none, u16 n <= 14, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

static const idx_t MAX_PRECOMP = 10000;
static bool initialized = false;
static int *phi      = NULL;   /* φ(0..MAX_PRECOMP)          */
static seq_t *pow2   = NULL;   /* 2^i (i=0..MAX_PRECOMP)    */
static seq_t *B      = NULL;   /* B(k) values                */

/* Euclidean gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* Pre‑compute φ, powers of 2 and the B(k) values up to MAX_PRECOMP */
static void init_precomputation(void)
{
    phi = (int *)malloc((MAX_PRECOMP+1) * sizeof(int));
    pow2 = (seq_t *)malloc((MAX_PRECOMP+1) * sizeof(seq_t));
    B = (seq_t *)malloc((MAX_PRECOMP+1) * sizeof(seq_t));
    if (!phi || !pow2 || !B) return; /* allocation failure – leave uninit */

    /* φ[i] = i initially */
    for (idx_t i = 0; i <= MAX_PRECOMP; ++i) phi[i] = (int)i;
    for (idx_t p = 2; p <= MAX_PRECOMP; ++p) {
        if (phi[p] == (int)p) {               /* prime */
            for (idx_t j = p; j <= MAX_PRECOMP; j += p)
                phi[j] -= phi[j] / (int)p;
        }
    }
    /* powers of 2 */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_PRECOMP; ++i)
        pow2[i] = pow2[i-1] * (seq_t)2;

    /* B(0) = 1; for k>0, B(k) = (1/k)*Σ_{d|k} φ(d)*2^{k/d} */
    B[0] = (seq_t)1;
    for (idx_t k = 1; k <= MAX_PRECOMP; ++k) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                sum += (seq_t)phi[d] * pow2[k / d];
                if (d != k / d)
                    sum += (seq_t)phi[k / d] * pow2[d];
            }
        }
        B[k] = sum / (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_precomputation();
        initialized = true;
    }
    if (n == 0) return B[0];      /* a(0) = 1 */

    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* Compute N(n,k) = (1/n)*Σ_{d|gcd(n,k)} φ(d)*binom(n/d,k/d) */
        idx_t g = gcd_idx(n, k);
        seq_t sumN = 0;
        for (idx_t d = 1; d * d <= g; ++d) {
            if (g % d == 0) {
                /* first divisor d */
                sumN += (seq_t)phi[d] * binom(n / d, k / d);
                /* second divisor g/d, if distinct */
                if (d != g / d)
                    sumN += (seq_t)phi[g / d] * binom(n / (g / d), k / (g / d));
            }
        }
        seq_t N_val = sumN / (seq_t)n;
        result += N_val * B[k];
    }
    return result;
}
