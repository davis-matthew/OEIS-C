/*
 * A061175  One half of sixth column of Lucas bisection triangle (odd part).
 * https://oeis.org/A061175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060924(n+5,5)/2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
# include <type_traits>
template<class T>
static inline T mul_small(T x, unsigned int m)
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(((__uint128_t)x) * m);
    } else {
        return x * static_cast<T>(m);
    }
}
#else
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return (seq_t)(((__uint128_t)x) * m);
}
#endif

seq_t a(idx_t n)
{
    /* Coefficients of (1-3x+x^2)^6 : d[0]..d[12]  */
    static const int64_t d[13] = {
        1,
        -18,
        141,
        -630,
        1770,
        -3258,
        3989,
        -3258,
        1770,
        -630,
        141,
        -18,
        1
    };

    /* Coefficients of N(x) = (1+x)(3-2x)(12x^3-35x^2+29x+1)(4x^3-9x^2+15x+3)  */
    static const int64_t c[9] = {
        9,     /* c0 */
        309,   /* c1 */
        1059,  /* c2 */
        -2121, /* c3 */
        445,   /* c4 */
        1359,  /* c5 */
        -1326, /* c6 */
        544,   /* c7 */
        -96    /* c8 */
    };

    seq_t buf[12] = {0};
    seq_t result = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        int kmax = (i < 12) ? (int)i : 12;
        for (int k = 1; k <= kmax; ++k) {
            unsigned int absdk = (d[k] > 0) ? (unsigned int)d[k] : (unsigned int)(-d[k]);
            seq_t prod = mul_small(buf[(i - k) % 12], absdk);
            if (d[k] > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }

        seq_t tmp = sum_neg;
        tmp -= sum_pos;                     /* tmp = - Σ d_k a_{n-k}  */

        int64_t ci = (i < 9) ? c[i] : 0;
        if (ci >= 0)
            tmp += (seq_t)ci;
        else
            tmp -= (seq_t)(-ci);

        result = tmp;
        buf[i % 12] = result;               /* store a_i for future use */
    }
    return result;
}
