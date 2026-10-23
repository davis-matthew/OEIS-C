/*
 * A042589  Denominators of continued fraction convergents to sqrt(823).
 * https://oeis.org/A042589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent to sqrt(823)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 21, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_k = A_k·p_{k-1} + p_{k-2}, q_k = A_k·q_{k-1} + q_{k-2}   */
/*             with p_{-2}=0, p_{-1}=1, q_{-2}=1, q_{-1}=0, A_0=28       */
/*             and A_{k+1} computed from the continued fraction of sqrt(823) */

#ifdef __cplusplus
#include <type_traits>
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safe_mul(T a, T b) {
    return (T)((unsigned __int128)a * (unsigned __int128)b);
}
template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
safe_mul(T a, T b) {
    return a * b;
}
#else
/* In C, use 128‑bit multiplication for integral types */
#define safe_mul(a,b) ((seq_t)((unsigned __int128)(a) * (unsigned __int128)(b)))
#endif

seq_t a(idx_t n) {
    /* Constants for sqrt(823) */
    const seq_t S = 823;          /* D in theory */
    const seq_t A0 = 28;          /* floor(sqrt(823)) */

    seq_t m = 0;                  /* m_0 */
    seq_t d = 1;                  /* d_0 */
    seq_t a_k = A0;               /* A_k, start with A_0 */

    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute current convergents */
        seq_t p = safe_mul(a_k, p_prev1) + p_prev2;
        seq_t q = safe_mul(a_k, q_prev1) + q_prev2;

        if (k == n) {
            result = q;
            break;
        }

        /* Shift the recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;

        /* Compute next continued fraction term A_{k+1} */
        seq_t m_next = safe_mul(d, a_k) - m;
        seq_t numer = S - safe_mul(m_next, m_next);   /* S - m_next^2 */
        seq_t d_next = numer / d;
        seq_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }
    return result;
}
