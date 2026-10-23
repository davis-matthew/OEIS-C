/*
 * A047773  Number of dissectable polyhedra with n tetrahedral cells and
 *          symmetry of type D.
 * https://oeis.org/A047773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of z^n in (G(z^6)-1)/z + z*G(z^6) - z +
 *            z^2*G(z^6)^2 + z^4*G(z^6)^2 - z^5*G(z^24) - z^17*G(z^24)^2 -
 *            (z^2*G(z^6) + z^2*G(z^12) + z^8*G(z^12)^2)/2 where G(z)
 *            satisfies G = 1 + z*G^3 (Fuss‑Catalan numbers). The function
 *            below computes a(n) for any n ≥ 1 by exploiting explici...
 * Range:     u8 none, u16 n <= 40, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const idx_t MAX_N = 10000;          /* maximum n to be handled */

static seq_t *g = NULL;       /* g[k] = coefficient of z^{6k} in G(z)   */
static seq_t *C6 = NULL;      /* C6[k] = coefficient of z^{6k} in G(z^6)^2    */
static seq_t *C12 = NULL;     /* C12[k] = coefficient of z^{12k} in G(z^12)^2 */
static seq_t *C24 = NULL;     /* C24[k] = coefficient of z^{24k} in G(z^24)^2 */

static void precompute(void)
{
    if (g != NULL) return;                /* already done */

    /* sizes for the required arrays */
    const size_t g_size  = (size_t)(MAX_N / 6) + 1;          /* indexes up to floor(MAX_N/6)  */
    const size_t c6_size = g_size;                          /* same size as g for C6          */
    const size_t c12_size= (size_t)(MAX_N / 12) + 1;
    const size_t c24_size= (size_t)(MAX_N / 24) + 1;

    /* allocate memory */
    g   = (seq_t*)malloc(g_size * sizeof(seq_t));
    C6  = (seq_t*)malloc(c6_size * sizeof(seq_t));
    C12 = (seq_t*)malloc(c12_size * sizeof(seq_t));
    C24 = (seq_t*)malloc(c24_size * sizeof(seq_t));

    /* g[0] = 1 (G(0) = 1) */
    g[0] = (seq_t)1;

    /* compute g[k] for k = 1 .. g_size-1 using the Fuss–Catalan formula:
         g[k] = (1/(2k+1)) * binom(3k, k)          */
    for (size_t k = 1; k < g_size; ++k) {
        seq_t res = (seq_t)1;
        /* binomial coefficient binom(3k, k) */
        for (size_t j = 1; j <= k; ++j) {
            res = (res * (seq_t)(3 * k - (j - 1))) / (seq_t)j;
        }
        /* divide by (2k + 1) */
        g[k] = res / (seq_t)(2 * k + 1);
    }

    /* convolution for C6: C6[i] = sum_{j=0..i} g[j]*g[i-j]  (i < c6_size) */
    for (size_t i = 0; i < c6_size; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j <= i; ++j)
            sum += g[j] * g[i - j];
        C6[i] = sum;
    }

    /* convolution for C12: uses g of index < c12_size  */
    for (size_t i = 0; i < c12_size; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j <= i; ++j)
            sum += g[j] * g[i - j];
        C12[i] = sum;
    }

    /* convolution for C24: uses g of index < c24_size  */
    for (size_t i = 0; i < c24_size; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j <= i; ++j)
            sum += g[j] * g[i - j];
        C24[i] = sum;
    }
}

/*--------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    /* initialise tables on first call */
    static int initialized = 0;
    if (!initialized) {
        precompute();
        initialized = 1;
    }

    /* use size_t for indices (n <= MAX_N) */
    size_t N = (size_t)n;

    /* accumulate positive and negative parts in order to avoid
       intermediate signed arithmetic.  All values are non‑negative
       uint types.  The final difference pos - neg is guaranteed to
       be non‑negative, as it equals 2*a(n).                 */
    seq_t pos = (seq_t)0;      /* positive contributions  (coeff * 2) */
    seq_t neg = (seq_t)0;      /* negative contributions  (coeff * 2) */

    /* -------- positive terms -------- */

    /* (G(z^6)-1)/z  -->  2*g[(n+1)/6] when (n+1)%6==0 */
    if ((N + 1) % 6 == 0) {
        size_t k = (N + 1) / 6;
        pos += (seq_t)2 * g[k];
    }

    /* z*G(z^6)      -->  2*g[(n-1)/6] when (n-1)%6==0 */
    if ((N >= 1) && ((N - 1) % 6 == 0)) {
        size_t k = (N - 1) / 6;
        pos += (seq_t)2 * g[k];
    }

    /* z^2*G(z^6)^2 -->  2*C6[(n-2)/6] when (n-2)%6==0 */
    if (N >= 2 && (N - 2) % 6 == 0) {
        size_t k = (N - 2) / 6;
        pos += (seq_t)2 * C6[k];
    }

    /* z^4*G(z^6)^2 -->  2*C6[(n-4)/6] when (n-4)%6==0 */
    if (N >= 4 && (N - 4) % 6 == 0) {
        size_t k = (N - 4) / 6;
        pos += (seq_t)2 * C6[k];
    }

    /* -------- negative terms -------- */

    /* -z            --> -(2) when n==1 */
    if (N == 1)
        neg += (seq_t)2;

    /* -z^5*G(z^24)  --> -2*g[(n-5)/24] when (n-5)%24==0 */
    if (N >= 5 && (N - 5) % 24 == 0) {
        size_t k = (N - 5) / 24;
        neg += (seq_t)2 * g[k];
    }

    /* -z^17*G(z^24)^2 --> -2*C24[(n-17)/24] when (n-17)%24==0 */
    if (N >= 17 && (N - 17) % 24 == 0) {
        size_t k = (N - 17) / 24;
        neg += (seq_t)2 * C24[k];
    }

    /* -(1/2)*z^2*G(z^6)  --> -g[(n-2)/6] when (n-2)%6==0 */
    if (N >= 2 && (N - 2) % 6 == 0) {
        size_t k = (N - 2) / 6;
        neg += g[k];
    }

    /* -(1/2)*z^2*G(z^12) --> -g[(n-2)/12] when (n-2)%12==0 */
    if (N >= 2 && (N - 2) % 12 == 0) {
        size_t k = (N - 2) / 12;
        neg += g[k];
    }

    /* -(1/2)*z^8*G(z^12)^2 --> -C12[(n-8)/12] when (n-8)%12==0 */
    if (N >= 8 && (N - 8) % 12 == 0) {
        size_t k = (N - 8) / 12;
        neg += C12[k];
    }

    /* final result: a(n) = (pos - neg) / 2  (dividing by 2 is exact) */
    seq_t result = (pos - neg) / (seq_t)2;
    return result;
}
