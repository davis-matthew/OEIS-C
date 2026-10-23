/*
 * A024387  [ (4th elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 1 mod 4}.
 * https://oeis.org/A024387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A024380(n) / A000384(n + 3))
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where
 *   A024380(n) = 4th elementary symmetric function of S(n)
 *   S(n)      = { 1,5,9,…,4(n+2)+1 }  (first n+3 numbers ≡ 1 (mod 4))
 *   A000384(k)= sum of the first k integers of the same progression
 *
 *  Computing e1 = Σ aᵢ  and e4 = Σ_{i<j<k<l} aᵢ aⱼ a_k a_l  via Newton's
 *  identities and power sums of the arithmetic progression.
 */
seq_t a(idx_t n)
{
    /* Number of elements in the set S(n) */
    seq_t m   = (seq_t)n + 3;     /* m = n+3  (>=4 for n>=1) */
    seq_t m1  = m - 1;            /* m-1 */

    /* Power sums p_k = Σ (4j+1)^k,  j=0..m-1  (k=1..4) */
    seq_t p1 = m * (2*m - 1);                                     /* Σ aₙ  */

    /* Σ (4j+1)² = 16 Σ j² + 8 Σ j + m */
    seq_t p2 = ( ( ( (seq_t)8 * m * m1 * (2*m - 1) ) / 3 )
                 + 4 * m * m1
                 + m );                                            /* Σ aₙ² */

    /* Σ (4j+1)³ = 64 Σ j³ + 48 Σ j² + 12 Σ j + m */
    seq_t p3 = 16 * m * m * m1 * m1
               + 8 * m * m1 * (2*m - 1)
               + 6 * m * m1
               + m;                                               /* Σ aₙ³ */

    /* Helper sums needed for Σ aₙ⁴ */
    seq_t sum_j  = m * m1 / 2;                                 /* Σ j               */
    seq_t sum_j2 = ( m1 * m * (2*m - 1) ) / 6;                 /* Σ j²              */
    seq_t sum_j3 = sum_j * sum_j;                              /* Σ j³              */
    seq_t sum_j4 = ( m1 * m * (2*m - 1) *
                     ( (seq_t)3*m*m - 3*m - 1 ) ) / 30;         /* Σ j⁴              */

    /* Σ (4j+1)⁴ = 256 Σ j⁴ + 256 Σ j³ + 96 Σ j² + 16 Σ j + m */
    seq_t p4 = 256 * sum_j4
               + 256 * sum_j3
               + 96  * sum_j2
               + 16  * sum_j
               + m;                                            /* Σ aₙ⁴ */

    /* Elementary symmetric sums via Newton's identities */
    seq_t e1 = p1;                                     /* e₁ = Σ aᵢ */
    seq_t e2 = (p1 * p1 - p2) / 2;                    /* e₂   */
    seq_t e3 = (e2 * p1 - e1 * p2 + p3) / 3;           /* e₃   */
    seq_t e4 = (e3 * p1 - e2 * p2 + e1 * p3 - p4) / 4;/* e₄   */

    /* Final answer: floor(e₄ / e₁) */
    return e4 / e1;
}
