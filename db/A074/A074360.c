/*
 * A074360  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,2).
 * https://oeis.org/A074360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074360 – coefficient of q^3 in the polynomials  nu(n)  with
 *  b = 2, lambda = 2.
 *  Recurrence for nu(n):
 *     nu(0) = 1
 *     nu(1) = b
 *     nu(n) = b·nu(n‑1) + λ·(1 + q + … + q^(n‑2))·nu(n‑2)      (n ≥ 2)
 *
 *  Let c_k(n) be the coefficient of q^k in nu(n).  For k = 0…3 we have
 *  (derived by collecting the coefficient of q^k in the recurrence):
 *
 *     c_0(n) = b·c_0(n‑1) + λ·c_0(n‑2)
 *     c_1(n) = b·c_1(n‑1) + λ·(c_1(n‑2) + c_0(n‑2))          if n‑2 ≥ 1
 *             = b·c_1(n‑1) + λ·c_1(n‑2)                        otherwise
 *     c_2(n) = b·c_2(n‑1) + λ·(c_2(n‑2) + c_1(n‑2) + c_0(n‑2))
 *             (note the terms omitted for n‑2 < 1 or 2)
 *     c_3(n) = b·c_3(n‑1) + λ·(c_3(n‑2) + c_2(n‑2) + c_1(n‑2) + c_0(n‑2))
 *             (terms omitted when n‑2 < 3)
 *
 *  Initial values:
 *     c_0(0) = 1 ,  c_1(0) = c_2(0) = c_3(0) = 0
 *     c_0(1) = 2 ,  c_1(1) = c_2(1) = c_3(1) = 0
 *
 *  Only the 0‑based index is used – a(0) is the first listed term (0).
 */

seq_t a(idx_t n)
{
    /* constants of the recurrence */
    const seq_t B = (seq_t)2;   /* b */
    const seq_t L = (seq_t)2;   /* λ */

    /* we compute the required coefficients iteratively,
       no memory allocation is needed.  All variables are of type
       seq_t to cooperate with arbitrary‑precision builds. */
    seq_t c0_prev2 = (seq_t)1, c1_prev2 = (seq_t)0, c2_prev2 = (seq_t)0, c3_prev2 = (seq_t)0;  /* n = 0 */
    if (n == 0) return (seq_t)0;                                                   /* a(0)=0 */

    seq_t c0_prev1 = (seq_t)2, c1_prev1 = (seq_t)0, c2_prev1 = (seq_t)0, c3_prev1 = (seq_t)0;  /* n = 1 */
    if (n == 1) return (seq_t)0;                                                   /* a(1)=0 */

    seq_t c0, c1, c2, c3;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t m = i - 2;      /* n‑2 for the current step */

        /* c0(i) */
        c0 = B * c0_prev1 + L * c0_prev2;

        /* c1(i) – sum over i=0..min(1,m) of c_{1‑i}(n‑2) */
        seq_t sum1 = (m >= 1) ? (c1_prev2 + c0_prev2) : c1_prev2;
        c1 = B * c1_prev1 + L * sum1;

        /* c2(i) – sum over i=0..min(2,m) of c_{2‑i}(n‑2) */
        seq_t sum2;
        if (m >= 2)
            sum2 = c2_prev2 + c1_prev2 + c0_prev2;
        else if (m == 1)
            sum2 = c2_prev2 + c1_prev2;
        else
            sum2 = c2_prev2;
        c2 = B * c2_prev1 + L * sum2;

        /* c3(i) – sum over i=0..min(3,m) of c_{3‑i}(n‑2) */
        seq_t sum3;
        if (m >= 3)
            sum3 = c3_prev2 + c2_prev2 + c1_prev2 + c0_prev2;
        else if (m == 2)
            sum3 = c3_prev2 + c2_prev2 + c1_prev2;
        else if (m == 1)
            sum3 = c3_prev2 + c2_prev2;
        else
            sum3 = c3_prev2;
        c3 = B * c3_prev1 + L * sum3;

        /* shift for next iteration */
        c0_prev2 = c0_prev1; c1_prev2 = c1_prev1; c2_prev2 = c2_prev1; c3_prev2 = c3_prev1;
        c0_prev1 = c0;       c1_prev1 = c1;       c2_prev1 = c2;       c3_prev1 = c3;
    }

    return c3_prev1;   /* a(n) = coefficient of q^3 in nu(n) */
}
