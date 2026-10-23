/*
 * A162010  Fifth right hand column of the EG1 triangle A162005.
 * https://oeis.org/A162010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of z^(n-1) in
 *            (7936+1305088*z-79792256*z^2-109331968*z^3+41828672000*z^4
 *            -460917924352*z^5+238697445120*z^6+5066784271872*z^7
 *            -14723693948160*z^8+12172737024000*z^9+8101522800000*z^10)
 *            divided by ((1-z)^5*(1-9*z)^4*(1-25*z)^3*(1-49*z)^2*(1-81*z))
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

/* ----------  helpers for multiplying a seq_t by a 128‑bit integer  ---------- */
#if defined(__cplusplus)
template <typename T>
static inline constexpr T mul_seq_int128(T a, __int128 b) {
    if constexpr (std::is_integral<T>::value) {
        return (T)((__int128)a * b);
    } else {
        return a * (T)b;   /* assumes the big integer type can be constructed from an int128 */
    }
}
#else
static inline seq_t mul_seq_int128(seq_t a, __int128 b) {
    return (seq_t)((__int128)a * b);
}
#endif

/* ----------  static tables for the rational generating function ---------- */
static __int128 D[16];          /* denominator coefficients: D[0]=1 */
static int64_t B[11];           /* numerator coefficients, B[0..10] */
static bool initialized = false;

/* ----------  initialise D and B  ---------- */
static void init_coeffs(void)
{
    if (initialized) return;
    /* -----  compute D(z) = (1-z)^5 (1-9z)^4 (1-25z)^3 (1-49z)^2 (1-81z)  ----- */
    __int128 poly[16] = {0};
    poly[0] = 1;
    int cur_len = 1;

    const int cs[5] = {1, 9, 25, 49, 81};   /* c in (1 - c z) */
    const int ks[5] = {5, 4, 3, 2, 1};       /* exponent k */

    for (int f = 0; f < 5; ++f) {
        int k = ks[f];
        int64_t c = cs[f];
        __int128 fac[6] = {0};
        fac[0] = 1;
        for (int t = 1; t <= k; ++t) {
            fac[t] = fac[t-1] * (k - t + 1) / t;   /* binomial part */
            fac[t] *= -c;                         /* multiply by (-c)^t */
        }
        /* convolution poly * fac */
        __int128 newpoly[16] = {0};
        for (int i = 0; i < cur_len; ++i)
            for (int j = 0; j <= k; ++j)
                newpoly[i + j] += poly[i] * fac[j];
        for (int i = 0; i < cur_len + k; ++i)
            poly[i] = newpoly[i];
        cur_len += k;
    }
    for (int i = 0; i < 16; ++i) D[i] = poly[i];

    /* -----  numerator coefficients  ----- */
    B[0]  =  7936;
    B[1]  = 1305088;
    B[2]  = -79792256;
    B[3]  = -109331968;
    B[4]  = 41828672000;
    B[5]  = -460917924352;
    B[6]  =  238697445120;
    B[7]  =  5066784271872;
    B[8]  = -14723693948160;
    B[9]  =  12172737024000;
    B[10] =   8101522800000;
    initialized = true;
}

/* ----------  memoised sequence buffer  ---------- */
static seq_t *memo = NULL;
static idx_t memo_len = 0;

/* -----  compute a single coefficient a_{idx}  ----- */
static seq_t compute_coeff(idx_t idx)
{
    /* for idx == 0 we have a(1) */
    size_t i = (size_t)idx;
    /* compute using recurrence: sum_{j=0}^{15} D[j] * a_{i-j} = B[i] (if i<=10) */
    seq_t pos = 0;   /* contributions to be added */
    seq_t neg = 0;   /* contributions to be subtracted */

    /* numerator part (only for i<=10) */
    if (i <= 10 && B[i] != 0) {
        __int128 bi = B[i];
        if (bi > 0)
            pos += (seq_t)bi;
        else
            neg += (seq_t)(-bi);
    }

    /* denominator part */
    for (int j = 1; j <= 15; ++j) {
        if (D[j] == 0) continue;
        if (i < (size_t)j) continue;          /* a_{negative} treated as 0 */
        seq_t term = mul_seq_int128(memo[i - j], D[j] > 0 ? D[j] : -D[j]);
        if (D[j] > 0)
            neg += term;
        else
            pos += term;
    }

    /* pos >= neg always holds because a_i is defined to be positive */
    return pos - neg;
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset <1, safe guard */
    init_coeffs();

    idx_t idx = n - 1;              /* zero‑based index */
    if (memo_len <= idx) {          /* extend cache up to idx */
        idx_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed */
        memo = tmp;
        for (idx_t k = memo_len; k < new_len; ++k)
            memo[k] = compute_coeff(k);
        memo_len = new_len;
    }
    return memo[idx];
}
