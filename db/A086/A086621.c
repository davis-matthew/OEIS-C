/*
 * A086621  Main diagonal of square table A086620 of coefficients, T(n,k), of
 *          x^n*y^k in f(x,y) that satisfies f(x,y) = 1/(1-x-y) + xy*f(x,y)^2.
 * https://oeis.org/A086621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n,j) * binomial(2*n-j,n) *
 *            binomial(2*j,j)/(j+1) (OEIS A086621)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *
 *   n*(n-1)*(n+1)^2*a(n) - 2*n*(n-1)*(4*n+3)*(2*n-1)*a(n-1)
 *   + 4*(n-1)*(16*n^3-20*n^2-13*n+14)*a(n-2)
 *   - 4*(n-2)*(4*n-9)*(4*n-3)*(n+1)*a(n-3) = 0
 *   (a(0)=1, a(1)=3, a(2)=14)                         */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
    #include <type_traits>
#endif

/* ------------------------------------------------------------------ */
/* Helper implementations ------------------------------------------- */
static seq_t compute_int128(idx_t n)
{
    /* 64‑bit builds only – uses unsigned __int128 for safety */
    if(n==0) return (seq_t)1;
    if(n==1) return (seq_t)3;
    if(n==2) return (seq_t)14;

    unsigned __int128 a_nm3 = 1;   /* a(t-3)  */
    unsigned __int128 a_nm2 = 3;   /* a(t-2)  */
    unsigned __int128 a_nm1 = 14;  /* a(t-1)  */
    unsigned __int128 a_n   = 0;

    for(idx_t i=3; i<=n; ++i) {
        unsigned __int128 i_t  = (unsigned __int128)i;
        unsigned __int128 i_m1 = i_t - 1;
        unsigned __int128 i_p1 = i_t + 1;

        /* termA = 2*i*(i-1)*(4*i+3)*(2*i-1)*a(t-1) */
        unsigned __int128 termA = (unsigned __int128)2 * i_t * i_m1 *
                                  (unsigned __int128)(4*i+3) *
                                  (unsigned __int128)(2*i-1) * a_nm1;

        /* polyB = 16*i^3-20*i^2-13*i+14 */
        unsigned __int128 polyB = (unsigned __int128)16 * i_t * i_t * i_t -
                                  (unsigned __int128)20 * i_t * i_t -
                                  (unsigned __int128)13 * i_t +
                                  (unsigned __int128)14;

        /* termB = 4*(i-1)*polyB*a(t-2) */
        unsigned __int128 termB = (unsigned __int128)4 * i_m1 * polyB * a_nm2;

        /* termC = 4*(i-2)*(4*i-9)*(4*i-3)*(i+1)*a(t-3) */
        unsigned __int128 termC = (unsigned __int128)4 * (i_t - 2) *
                                  (unsigned __int128)(4*i-9) *
                                  (unsigned __int128)(4*i-3) *
                                  i_p1 * a_nm3;

        /* denominator = i*(i-1)*(i+1)^2 */
        unsigned __int128 denom = i_t * i_m1 * i_p1 * i_p1;

        /* a(t) = (termA - termB + termC) / denom */
        unsigned __int128 numerator = termA - termB + termC;
        a_n = numerator / denom;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return (seq_t)a_nm1;
}

static seq_t compute_seq_t(idx_t n)
{
    /* Arbitrary‑precision builds only – uses seq_t for all ops */
    if(n==0) return (seq_t)1;
    if(n==1) return (seq_t)3;
    if(n==2) return (seq_t)14;

    seq_t a_nm3 = 1, a_nm2 = 3, a_nm1 = 14, a_n = 0;

    for(idx_t i=3; i<=n; ++i) {
        seq_t termA = (seq_t)2 * (seq_t)i * (seq_t)(i-1) *
                      (seq_t)(4*i+3) * (seq_t)(2*i-1) * a_nm1;

        seq_t polyB = (seq_t)16 * (seq_t)i * (seq_t)i * (seq_t)i -
                      (seq_t)20 * (seq_t)i * (seq_t)i -
                      (seq_t)13 * (seq_t)i + (seq_t)14;

        seq_t termB = (seq_t)4 * (seq_t)(i-1) * polyB * a_nm2;

        seq_t termC = (seq_t)4 * (seq_t)(i-2) *
                      (seq_t)(4*i-9) * (seq_t)(4*i-3) *
                      (seq_t)(i+1) * a_nm3;

        seq_t denom = (seq_t)i * (seq_t)(i-1) *
                      (seq_t)(i+1) * (seq_t)(i+1);

        seq_t numerator = termA - termB + termC;

        a_n = numerator / denom;

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

#ifdef __cplusplus
/* C++17: use constexpr if – decides at compile time whether seq_t is 64‑bit */
static seq_t a_impl(idx_t n)
{
    if constexpr (std::is_same<seq_t, uint64_t>::value ||
                  std::is_same<seq_t, unsigned long long>::value) {
        /* 64‑bit build – use u128 path */
        return compute_int128(n);
    } else {
        /* arbitrary‑precision build – use seq_t path */
        return compute_seq_t(n);
    }
}
#else /* C11 */
static seq_t a_impl(idx_t n)
{
    /* pick the implementation using _Generic on the type of seq_t */
    return _Generic((seq_t)0,
        uint64_t          : compute_int128(n),
        unsigned long long: compute_int128(n),
        /* all other types -> use seq_t arithmetic */
        default           : compute_seq_t(n));
}
#endif

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    return a_impl(n);
}
