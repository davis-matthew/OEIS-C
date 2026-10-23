/*
 * A124631  Poincaré series [or Poincare series] P(C#_{4,2}; x).
 * https://oeis.org/A124631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from N(x)/D(x)
 *            where N(x) = 1+10x⁴+12x⁵+38x⁶+46x⁷+93x⁸+131x⁹+235x¹⁰
 *            +299x¹¹+473x¹²+560x¹³+714x¹⁴+761x¹⁵+876x¹⁶
 *            +830x¹⁷+876x¹⁸+761x¹⁹+714x²⁰+560x²¹+473x²²
 *            +299x²³+235x²⁴+131x²⁵+93x²⁶+46x²⁷+38x²⁸+12x²⁹+10x³⁰+x³⁴ D(x) =
 *            (1‑x²)³(1‑x³)⁴(1‑x⁴)⁴(1‑x⁵)²(1‑x⁶)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n ≥ 0 :  a[n] = N_n  – Σ_{k=1}^{deg} D_k · a[n‑k]   */
/* D_0 = 1.  The N_n term is 0 for n ≥ 35.                         */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int D_coeff[49] = {0};  /* denominator coefficients, D_coeff[0] == 1 */
static int D_deg = 0;

static int N_coeff[35] = {0};  /* numerator coefficients */

static seq_t *cache = NULL;    /* cache of computed terms */
static size_t cache_cap = 0;   /* current allocated capacity */
static size_t cache_size = 0;  /* number of terms already computed */

/*------------------------------------------------------------------*/
/*  Multiplicative binomial coefficient for small integers.         */
/*------------------------------------------------------------------*/
static int binom_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    int res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/*------------------------------------------------------------------*/
/*  Build the denominator polynomial D(x).                          */
/*------------------------------------------------------------------*/
static void compute_denominator(void)
{
    int cur[50] = {0};
    int cur_deg = 0;
    cur[0] = 1;                        /* start with 1 */

    /* factors: (exponent, multiplicity) */
    const int factors[5][2] = { {2,3}, {3,4}, {4,4}, {5,2}, {6,1} };

    for (int f = 0; f < 5; ++f) {
        int k = factors[f][0];
        int m = factors[f][1];
        int f_arr[50] = {0};
        int f_deg = m * k;
        for (int i = 0; i <= m; ++i) {
            int coeff = binom_small(m, i);
            if (i & 1) coeff = -coeff;   /* (1 - x^k)^m */
            f_arr[i * k] = coeff;
        }

        int tmp[50] = {0};
        for (int i = 0; i <= cur_deg; ++i) {
            if (cur[i] == 0) continue;
            for (int j = 0; j <= f_deg; ++j) {
                if (f_arr[j] == 0) continue;
                tmp[i + j] += cur[i] * f_arr[j];
            }
        }
        int new_deg = cur_deg + f_deg;
        for (int i = 0; i <= new_deg; ++i) cur[i] = tmp[i];
        cur_deg = new_deg;
    }

    for (int i = 0; i <= cur_deg; ++i) D_coeff[i] = cur[i];
    D_deg = cur_deg;   /* should be 48 */
}

/*------------------------------------------------------------------*/
/*  Build the numerator polynomial N(x).                            */
/*------------------------------------------------------------------*/
static void compute_numerator(void)
{
    N_coeff[0]  = 1;
    N_coeff[4]  = 10;
    N_coeff[5]  = 12;
    N_coeff[6]  = 38;
    N_coeff[7]  = 46;
    N_coeff[8]  = 93;
    N_coeff[9]  = 131;
    N_coeff[10] = 235;
    N_coeff[11] = 299;
    N_coeff[12] = 473;
    N_coeff[13] = 560;
    N_coeff[14] = 714;
    N_coeff[15] = 761;
    N_coeff[16] = 876;
    N_coeff[17] = 830;
    N_coeff[18] = 876;
    N_coeff[19] = 761;
    N_coeff[20] = 714;
    N_coeff[21] = 560;
    N_coeff[22] = 473;
    N_coeff[23] = 299;
    N_coeff[24] = 235;
    N_coeff[25] = 131;
    N_coeff[26] = 93;
    N_coeff[27] = 46;
    N_coeff[28] = 38;
    N_coeff[29] = 12;
    N_coeff[30] = 10;
    N_coeff[34] = 1;   /* x^34 term */
}

/*------------------------------------------------------------------*/
/*  One‑time initialization.                                        */
/*------------------------------------------------------------------*/
static void init_polynomials(void)
{
    if (D_deg != 0) return;   /* already done */
    compute_denominator();
    compute_numerator();
}

/*------------------------------------------------------------------*/
/*  Ensure the cache can hold terms up to index n.                  */
/*------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);    /* allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;
}

/*------------------------------------------------------------------*/
/*  Sequence function.                                              */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init_polynomials();
    ensure_cache(n);

    /* Compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t rhs = 0;
        if (i < 35) rhs = (seq_t)N_coeff[i];

        seq_t ans = rhs;
        size_t maxk = (i <= (size_t)D_deg) ? i : (size_t)D_deg;
        for (size_t k = 1; k <= maxk; ++k) {
            int coeff = D_coeff[(size_t)k];
            if (coeff == 0) continue;
            seq_t val = cache[(size_t)i - k];
            if (coeff > 0)
                ans -= (seq_t)coeff * val;
            else
                ans += (seq_t)(-coeff) * val;
        }
        cache[(size_t)i] = ans;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
