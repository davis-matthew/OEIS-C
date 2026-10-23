/*
 * A042745  Denominators of continued fraction convergents to sqrt(903).
 * https://oeis.org/A042745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the denominator q_n of the n‑th convergent of the continued
 * fraction for sqrt(903).  Let a0 = ⌊sqrt(903)⌋ = 30.  The partial quotients a_k
 * for k≥1 are obtained by the standard algorithm
 *   m_{k+1} = d_k * a_k – m_k
 *   d_{k+1} = (903 – m_{k+1}²) / d_k
 *   a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋
 * with m_0=0, d_0=1, a_0=a0.
 * The convergents satisfy
 *   q_{-1}=1,   q_0 = 1,
 *   q_1   = a_1,
 *   q_n   = a_n * q_{n-1} + q_{n-2}   for n ≥ 2.
 *
 * The function below computes a(n) directly from the definitions above
 * and works for any unsigned integer type seq_t and for any unsigned
 * integer type idx_t (uint64_t).  No external memory allocation is
 * required, so it is safe for both C11 and C++17.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants */
    const uint32_t D   = 903;          /* D = 903 */
    const uint32_t A0  = 30;           /* a0 = floor(sqrt(903)) */

    if (n == 0) {
        return (seq_t)1;              /* q0 = 1 */
    }

    /* first non‑trivial partial quotient a1 */
    uint64_t m   = 0;
    uint64_t d   = 1;
    uint32_t a   = A0;

    /* compute a1 */
    uint64_t m1   = d * a - m;          /* = 30 */
    uint64_t d1   = (D - m1 * m1) / d;  /* = 3 */
    uint32_t a1   = (A0 + m1) / d1;     /* = 20 */

    if (n == 1) {
        return (seq_t)a1;              /* q1 = a1 = 20 */
    }

    /* initial convergents q0 and q1 */
    seq_t q_nm2 = (seq_t)1;            /* q0 */
    seq_t q_nm1 = (seq_t)a1;           /* q1 */

    /* set state for the next partial quotient */
    m = m1;
    d = d1;
    a = a1;

    /* iterate to reach q_n */
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t m_next = d * a - m;                   /* m_{k+1} */
        uint64_t d_next = (D - m_next * m_next) / d;   /* d_{k+1} */
        uint32_t a_next = (A0 + m_next) / d_next;      /* a_{k+1} */

        seq_t q_n = (seq_t)a_next * q_nm1 + q_nm2;     /* q_k */

        /* shift convergents */
        q_nm2 = q_nm1;
        q_nm1 = q_n;

        /* update continued‑fraction state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return q_nm1;            /* q_n */
}
