/*
 * A042071  Denominators of continued fraction convergents to sqrt(559).
 * https://oeis.org/A042071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction denominators of sqrt(559).  
   For sqrt(559) the simple continued fraction has period length 20
   with coefficients  
   (a0; a1 … a20) = (23; 1,1,1,4,15,1,1,4,1,2,1,4,1,1,15,4,1,1,1,46).  
   The denominators q_n satisfy the recurrence  
       q_n = a_n * q_{n-1} + q_{n-2}  
   with initial values q_{-2} = 1, q_{-1} = 0.  */

/* Computes a(n), the denominator of the nth convergent to sqrt(559). */
seq_t a(idx_t n)
{
    /* Partial quotients for the periodic part (a1 … a20).  */
    static const seq_t period[20] = {
        1, 1, 1, 4, 15, 1, 1, 4, 1, 2,
        1, 4, 1, 1, 15, 4, 1, 1, 1, 46
    };
    const seq_t a0 = 23;          /* a0 = floor(sqrt(559)) */

    seq_t q_prev2 = 1;   /* q_{-2} */
    seq_t q_prev1 = 0;   /* q_{-1} */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)
            coeff = a0;
        else
            coeff = period[(i - 1) % 20];
        seq_t q = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
