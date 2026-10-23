/*
 * A180399  Expansion of (1/3)*(1 - (1-9*x-9*x^2)^(1/3)).
 * https://oeis.org/A180399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence * a(0) = 0, a(1) = 1, a(2) = 4 * for n ≥ 3: * n*a(n)
 *            = 3*(3*n-4)*a(n-1) + 3*(3*n-8)*a(n-2) * This implements the
 *            recurrence using only integer arithmetic. * In C (built‑in
 *            integer seq_t) 128‑bit intermediates avoid overflow. * In C++
 *            (arbitrary‑precision seq_t) the operations are perfo...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build with big‑integer seq_t */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;

    seq_t a_nm2 = 1; /* a(1) */
    seq_t a_nm1 = 4; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t ni = seq_t(i);

        seq_t term1 = seq_t(3) * seq_t(3 * i - 4) * a_nm1;
        seq_t term2 = seq_t(3) * seq_t(3 * i - 8) * a_nm2;
        seq_t cur   = (term1 + term2) / ni;

        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
#else
    /* C build with built‑in unsigned integer seq_t */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;

    seq_t a_nm2 = 1; /* a(1) */
    seq_t a_nm1 = 4; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        unsigned __int128 t1 = (unsigned __int128)3 *
                               (unsigned __int128)(3 * i - 4) *
                               (unsigned __int128)a_nm1;

        unsigned __int128 t2 = (unsigned __int128)3 *
                               (unsigned __int128)(3 * i - 8) *
                               (unsigned __int128)a_nm2;

        unsigned __int128 cur128 = (t1 + t2) / i;

        a_nm2 = a_nm1;
        a_nm1 = (seq_t)cur128;
    }
    return a_nm1;
#endif
}
