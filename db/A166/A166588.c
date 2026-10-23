/*
 * A166588  Partial sums of A097331; binomial transform of A166587.
 * https://oeis.org/A166588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*A166587(k). Recurrence (valid for n ≥
 *            3): (-n-1)*a(n) + (n+1)*a(n-1) + 4*(n-2)*a(n-2) +
 *            4*(-n+2)*a(n-3) = 0 Initial values: a(0)=1, a(1)=2, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 40, u64 n <= 74, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;

    /* a(n-3), a(n-2), a(n-1) for the recurrence. */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)2;   /* a(1) */
    seq_t a_nm1 = (seq_t)2;   /* a(2) */
    seq_t a_cur = (seq_t)0;

#ifdef __cplusplus
    /* C++20+ allows constexpr-if, but we need C++17 support. */
    using Integral = typename std::is_integral<seq_t>::type;
    if constexpr (std::is_integral<seq_t>::value) {
        /* Built-in integral type: use 128‑bit intermediate to avoid overflow. */
        for (idx_t i = 3; i <= n; ++i) {
            seq_t diff = a_nm2 - a_nm3;                      /* a(n-2)-a(n-3) */
            unsigned __int128 tmp = (unsigned __int128)diff * 4 * (i - 2);
            a_cur = a_nm1 + (seq_t)(tmp / (i + 1));
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_cur;
        }
    } else {
        /* Arbitrary‑precision type: use seq_t arithmetic. */
        for (idx_t i = 3; i <= n; ++i) {
            seq_t diff = a_nm2 - a_nm3;
            seq_t term = ((diff << 2) * (seq_t)(i - 2)) / (seq_t)(i + 1);
            a_cur = a_nm1 + term;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_cur;
        }
    }
#else
    /* C implementation: assume seq_t is an unsigned integral type. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t diff = a_nm2 - a_nm3;
        unsigned __int128 tmp = (unsigned __int128)diff * 4 * (i - 2);
        a_cur = a_nm1 + (seq_t)(tmp / (i + 1));
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
#endif

    return a_cur;
}
