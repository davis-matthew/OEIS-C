/*
 * A015044  Inverse of 1035th cyclotomic polynomial.
 * https://oeis.org/A015044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of xⁿ in the power‑series expansion of *
 *            the reciprocal of the 1035‑th cyclotomic polynomial * Φ₁₀₃₅⁻¹(x)
 *            . * The sequence is periodic with period 1035
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1831 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
static void divide_poly(const long long *num, int deg_num,
                        const long long *div, int deg_div,
                        long long *quo, int *deg_quo)
{
    /*  Work on a temporary copy of the dividend  */
    long long *rem = (long long *)malloc((deg_num + 1) * sizeof(long long));
    for (int i = 0; i <= deg_num; ++i)
        rem[i] = num[i];

    int qdeg = deg_num - deg_div;
    for (int i = qdeg; i >= 0; --i) {
        long long coeff = rem[i + deg_div];
        quo[i] = coeff;

        for (int j = 0; j <= deg_div; ++j) {
            rem[i + j] -= coeff * div[j];
        }
    }

    free(rem);

    /*  Trim possible leading zeros  */
    int actual_deg = qdeg;
    while (actual_deg > 0 && quo[actual_deg] == 0)
        --actual_deg;
    *deg_quo = actual_deg;
}

/* -------------------------------------------------------------------------- */
static int compute_phi_rec(uint64_t n, long long **out, int *deg)
{
    if (n == 1) {                     /* Φ₁(x) = x – 1  */
        *deg = 1;
        *out = (long long *)malloc(2 * sizeof(long long));
        (*out)[0] = -1;
        (*out)[1] = 1;
        return 1;
    }

    /*  List all proper divisors of n (excluding n itself)  */
    uint64_t divisors[128];
    size_t div_cnt = 0;
    for (uint64_t d = 1; d <= n / 2; ++d)
        if (n % d == 0)
            divisors[div_cnt++] = d;

    /*  Start with xⁿ – 1  */
    int deg_num  = (int)n;
    long long *num = (long long *)calloc(deg_num + 1, sizeof(long long));
    num[0] = -1;
    num[n] = 1;

    /*  Divide successively by Φ_d for each proper divisor d  */
    for (size_t k = 0; k < div_cnt; ++k) {
        uint64_t d = divisors[k];
        long long *phi_d = NULL;
        int deg_d = 0;
        compute_phi_rec(d, &phi_d, &deg_d);

        int deg_q = deg_num - deg_d;
        long long *quo = (long long *)calloc(deg_q + 1, sizeof(long long));

        divide_poly(num, deg_num, phi_d, deg_d, quo, &deg_q);

        free(num);
        free(phi_d);

        num      = quo;
        deg_num  = deg_q;
    }

    *deg = deg_num;
    *out = num;
    return 1;
}

/* -------------------------------------------------------------------------- */
static void init_cache(seq_t *cache)
{
    const int PERIOD = 1035;
    static long long phi[529];   /* φ(1035) = 528  */
    static int  phi_deg = 0;

    /*  Compute Φ₁₀₃₅  */
    long long *phi_arr = NULL;
    compute_phi_rec(1035, &phi_arr, &phi_deg);

    /*  Copy to static array for fast access  */
    for (int i = 0; i <= phi_deg; ++i)
        phi[i] = phi_arr[i];

    free(phi_arr);

    /*  Compute the periodic sequence using the recurrence  */
    cache[0] = (seq_t)1;                /* a(0) = 1 */
    for (int n = 1; n < PERIOD; ++n) {
        long long sum = 0;
        int max_i = (n < phi_deg) ? n : phi_deg;
        for (int i = 1; i <= max_i; ++i) {
            sum += phi[i] * (long long)cache[n - i];
        }
        cache[n] = (seq_t)(-sum);
    }
}

/* Forward declarations of helper functions */
static int  compute_phi_rec(uint64_t n, long long **out, int *deg);
static void divide_poly(const long long *num, int deg_num,
                        const long long *div, int deg_div,
                        long long *quo, int *deg_quo);

/* -------------------------------------------------------------------------- */
/*  Recursive computation of a single cyclotomic polynomial Φ_n   */

/* -------------------------------------------------------------------------- */
/*  Generic polynomial division (num / div)  */
/*  Both polynomials are monic; division is exact.         */

/* -------------------------------------------------------------------------- */
/*  One‑time initialization of the period and cache  */

/* -------------------------------------------------------------------------- */
/*  Required public function  */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t cache[1035];
    static int  initialized = 0;

    if (!initialized) {
        init_cache(cache);
        initialized = 1;
    }
    const uint64_t PERIOD = 1035;
    return cache[(unsigned)(n % PERIOD)];
}
