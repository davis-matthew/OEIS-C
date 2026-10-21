/*
 * A055330  Number of rooted identity trees with n nodes and 5 leaves.
 * https://oeis.org/A055330
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -∑ₖ₌₁²⁰ rec[k] · a(n–k) for n ≥ 30 The recurrence
 *            coefficients rec[k] are the coefficients of the denominator Q(x)
 *            = (1–x)⁹ · (1+x)³ · (1+x²) · (1+x+x²) · (1+x+x²+x³+x⁴) of the
 *            generating function. The first 20 terms ( for 10 ≤ n ≤ 29 ) are
 *            used as initial values. The function builds the c...
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 65, u64 n <= 984,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Type definitions supplied by the build system */
#include <stdint.h>   /* for int64_t */

/* --- configuration ---------------------------------------------------- */
#define REC_ORDER   20         /* order of the recurrence            */
#define MAX_CACHE  20000        /* enough for n up to ~10 000         */

/* --- recurrence coefficients (Q(x) coefficients) --------------------- */
static int64_t rec[REC_ORDER + 1];   /* rec[0] … rec[20], rec[0] == 1 */

/* flag to build the coefficients only once */
static int rec_init_done = 0;

/* multiply two small polynomials (a of degree da, b of degree db).
   The result is stored in 'res', whose size is at least da + db + 1. */
static void poly_mul(const int64_t *a, int da,
                     const int64_t *b, int db,
                     int64_t *res, int *dres)
{
    int i, j;
    for (int k = 0; k <= da + db; ++k) res[k] = 0;
    for (i = 0; i <= da; ++i)
        for (j = 0; j <= db; ++j)
            res[i + j] += a[i] * b[j];
    *dres = da + db;
}

/* build the recurrence coefficients once */
static void init_rec(void)
{
    if (rec_init_done) return;
    rec_init_done = 1;

    /* (1–x)⁹  coeffs */
    const int64_t poly1[10] = {  1LL, -9LL, 36LL, -84LL, 126LL, -126LL,
                                84LL, -36LL,  9LL, -1LL };

    /* (1+x)³   coeffs */
    const int64_t poly2[4]  = { 1LL, 3LL, 3LL, 1LL };

    /* (1+x²)    coeffs */
    const int64_t poly3[3]  = { 1LL, 0LL, 1LL };

    /* (1+x+x²)  coeffs */
    const int64_t poly4[3]  = { 1LL, 1LL, 1LL };

    /* (1+x+x²+x³+x⁴) coeffs */
    const int64_t poly5[5]  = { 1LL, 1LL, 1LL, 1LL, 1LL };

    int d1 = 9, d2 = 3, d3 = 2, d4 = 2, d5 = 4;
    int64_t temp1[21], temp2[21];
    int d_tmp;

    /* poly1 * poly2 */
    poly_mul(poly1, d1, poly2, d2, temp1, &d_tmp);
    /* * poly3 */
    poly_mul(temp1, d_tmp, poly3, d3, temp2, &d_tmp);
    /* copy back */
    for (int i = 0; i <= d_tmp; ++i) temp1[i] = temp2[i];
    /* * poly4 */
    poly_mul(temp1, d_tmp, poly4, d4, temp2, &d_tmp);
    for (int i = 0; i <= d_tmp; ++i) temp1[i] = temp2[i];
    /* * poly5 */
    poly_mul(temp1, d_tmp, poly5, d5, temp2, &d_tmp);
    for (int i = 0; i <= d_tmp; ++i) temp1[i] = temp2[i];

    /* temp1 now contains the coefficients of Q(x) */
    for (int i = 0; i <= d_tmp; ++i)
        rec[i] = temp1[i];          /* rec[0] == 1 */
}

/* --- cache for computed terms ---------------------------------------- */
static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;   /* number of terms stored currently */

/* --- first 20 terms : n = 10 … 29 ---------------------------------- */
static const seq_t init_terms[REC_ORDER] = {
    (seq_t)3,        /* a(10) */
    (seq_t)26,       /* a(11) */
    (seq_t)116,      /* a(12) */
    (seq_t)387,      /* a(13) */
    (seq_t)1068,     /* a(14) */
    (seq_t)2587,     /* a(15) */
    (seq_t)5678,     /* a(16) */
    (seq_t)11540,    /* a(17) */
    (seq_t)22034,    /* a(18) */
    (seq_t)39957,    /* a(19) */
    (seq_t)69366,    /* a(20) */
    (seq_t)116009,   /* a(21) */
    (seq_t)187823,   /* a(22) */
    (seq_t)295574,   /* a(23) */
    (seq_t)453582,   /* a(24) */
    (seq_t)680625,   /* a(25) */
    (seq_t)1000952,  /* a(26) */
    (seq_t)1445516,  /* a(27) */
    (seq_t)2053343,  /* a(28) */
    (seq_t)2873165   /* a(29) */
};

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Undefined for n < 10; return 0 as a safe default. */
    if (n < 10) return (seq_t)0;

    /* index in cache (a(10) maps to cache[0]) */
    idx_t idx = n - 10;

    /* initialise cache with the first 20 terms on first call */
    if (cache_len == 0) {
        cache_len = REC_ORDER;
        for (idx_t i = 0; i < REC_ORDER; ++i)
            cache[i] = init_terms[i];
    }

    /* expand cache if necessary */
    if (idx >= cache_len) {
        init_rec();   /* ensure recurrence coefficients are ready */

        while (cache_len <= idx) {
            /* compute next term using recurrence:
               a(k) = - ∑_{j=1}^{20} rec[j] * a(k-j) */
            signed __int128 sum = 0;
            for (int j = 1; j <= REC_ORDER; ++j) {
                signed __int128 term =
                    (signed __int128)rec[j] *
                    (signed __int128)cache[cache_len - j];
                sum -= term;
            }
            cache[cache_len] = (seq_t)sum;
            ++cache_len;
        }
    }

    return cache[idx];
}
