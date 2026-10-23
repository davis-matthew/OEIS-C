/*
 * A144409  Antidiagonal expansion of: f(t,n) = If[n == 1, 1/(1 - t), 1/(1 -
 *          t^floor(n/2) - t^n)].
 * https://oeis.org/A144409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) is the antidiagonal expansion of f(t,n) = If[n==1, 1/(1‑t),
 *            1/(1‑t^floor(n/2)‑t^n) ]. The nth term is obtained by locating
 *            the triangular diagonal that contains n, determining the
 *            parameters (n_func , m_exp) and computing the coefficient of
 *            t^m_exp in the expansion of 1/(1‑t^floor(n_func/2)‑...
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of range – not used */

    /* Find diagonal s such that 1+2+...+(s+1) >= n. */
    idx_t s = 0;
    while (((unsigned __int128)(s + 1) * (s + 2)) / 2 < n)
        ++s;

    idx_t prev_total = ((unsigned __int128)s * (s + 1)) / 2;
    idx_t offset     = n - prev_total;          /* 1‑based position in the diagonal */

    /* Parameters for the generating function: f(t, n_func) */
    idx_t n_func = s + 2 - offset;              /* n in f(t,n)          */
    idx_t m_exp  = offset - 1;                  /* exponent of t to read */

    /* f(t,1) = 1/(1‑t) expands to all 1’s. */
    if (n_func == 1)
        return (seq_t)1;

    idx_t a_step = n_func / 2;                  /* floor(n_func/2)      */
    idx_t b_step = n_func;                      /* exponent n_func      */

    /* Maximal m_exp for n up to about 10 000 is < 200, so a fixed buffer
       is sufficient for required time limits. */
    seq_t C[2048];
    C[0] = (seq_t)1;
    for (idx_t i = 1; i <= m_exp; ++i) {
        seq_t val = (i >= a_step ? C[i - a_step] : (seq_t)0);
        if (i >= b_step)
            val = val + C[i - b_step];
        C[i] = val;
    }

    return C[m_exp];
}
