/*
 * A105611  a(n) is the LCM of the Jacobsthal sequence {J(1),...,J(n)}.
 * https://oeis.org/A105611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm_{k=1..n} J(k) , J(0)=0, J(1)=1, J(k)=J(k-1)+2*J(k-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    /* Euclidean algorithm */
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below offset */

    /* Jacobsthal numbers: J(1)=1, J(2)=1 */
    seq_t J_prev2 = 1;            /* J(1)   */
    seq_t J_prev1 = 1;            /* J(2)   */

    /* LCM accumulator */
    seq_t lcm_val = 1;            /* lcm of {J(1)} */

    if (n == 1) return lcm_val;   /* a(1) */

    /* include J(2) */
    seq_t g = gcd_seq(lcm_val, J_prev1);
    lcm_val = (lcm_val / g) * J_prev1;   /* still 1 */

    if (n == 2) return lcm_val;   /* a(2) */

    /* iterate for k = 3 .. n */
    for (idx_t k = 3; k <= n; ++k) {
        /* J(k) = J(k-1) + 2*J(k-2) */
        seq_t J_cur = J_prev1 + (J_prev2 * (seq_t)2);

        g = gcd_seq(lcm_val, J_cur);
        lcm_val = (lcm_val / g) * J_cur;

        J_prev2 = J_prev1;
        J_prev1 = J_cur;
    }
    return lcm_val;
}
