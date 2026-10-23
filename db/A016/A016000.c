/*
 * A016000  Inverse of 1991st cyclotomic polynomial.
 * https://oeis.org/A016000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(0)=1; for n>0 a(n) = -∑_{k=1}^{φ(1991)} φ_k * a(n-k), where
 *            φ_k are the integer coefficients of the cyclotomic polynomial
 *            Φ_{1991}(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The product 1991 = 11 · 181.  All constants below are
   integer constant expressions so that array sizes are
   valid at file scope. */
#define P   11
#define Q   181
#define PQ  (P*Q)

#define NUM_DEG (PQ + 1)   /* degree of (x-1)(x^{1991}-1)  */
#define DEN_DEG (P + Q)    /* degree of (x^11-1)(x^181-1) */
#define PHI_DEG 1800       /* φ(1991)                       */
#define PHI_LEN (PHI_DEG + 1)

/* Coefficients of Φ_{1991}(x); phi[0] is the constant term
   (equal to 1).  The array is filled on first use. */
static seq_t phi[PHI_LEN];
static int phi_initialized = 0;

/* Forward declaration of the helper that builds the
   coefficients of Φ_{1991}. */
static void init_phi(void)
{
    if (phi_initialized)
        return;

    /* Working arrays for the long division.  They are local
       to this function and thus of static storage duration. */
    int N[NUM_DEG + 1] = {0};
    int D[DEN_DEG + 1] = {0};

    /* (x-1)(x^{1991}-1) */
    N[0]     = 1;
    N[1]     = -1;
    N[PQ]    = -1;
    N[PQ + 1] = 1;

    /* (x^{11}-1)(x^{181}-1) */
    D[0]         = 1;
    D[P]         = -1;
    D[Q]         = -1;
    D[P + Q]     = 1;

    /* Long division: N(x) / D(x) produces Φ_{1991}(x). */
    for (int i = NUM_DEG; i >= DEN_DEG; --i) {
        int coeff = N[i];
        phi[i - DEN_DEG] = coeff;          /* store the quotient coeff. */

        if (coeff != 0) {
            for (int j = 0; j <= DEN_DEG; ++j) {
                if (D[j] != 0)
                    N[i - DEN_DEG + j] -= coeff * D[j];
            }
        }
    }

    phi_initialized = 1;
}

/* Cache for the sequence values a(n).  The cache size is fixed
   to 20000 which comfortably exceeds the ~10000 upper bound
   mentioned in the problem statement. */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;

/* Ensure that a(0) … a(n) are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    /* If we have already computed up to a(n) nothing to do. */
    if (cache_len > n)
        return;

    /* Guard against out‑of‑range indices – the problem limits n
       to about 10000, so CACHE_MAX is sufficient. */
    if ((size_t)n + 1 > CACHE_MAX)
        abort();

    init_phi();  /* make sure φ coefficients are ready */

    /* Start computing from the first missing term up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 1;      /* a(0) = 1 */
            continue;
        }

        seq_t sum = 0;
        size_t lim = (i < PHI_DEG) ? i : PHI_DEG;
        for (size_t k = 1; k <= lim; ++k)
            sum += phi[k] * cache[i - k];

        cache[i] = -sum;   /* a(n) = -∑ φ_k * a(n-k)   */
    }

    cache_len = n + 1;
}

/* ------------------------------------------------------------- */
/* Public function expected by the build system.                  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
