/*
 * A210620  Nontrivial solution to x = 2*x^2 - 1 mod 10^n.
 * https://oeis.org/A210620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements Hensel lifting for sequence A210620.
   a(1) = 7, and a(n+1) = d*10^n + a(n), where digit d is chosen such that
   2*x^2 - x - 1 ≡ 0 (mod 10^(n+1)).
   The digit d is computed from
       d ≡ -(A_n) * (4*a(n) - 1)^{-1}   (mod 10),
   where A_n = (2*a(n)^2 - a(n) - 1)/10^n.  */

seq_t a(idx_t n)
{
    if (n == 0)                      /* offset is 1, 0 is not required */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)7;

    seq_t x      = (seq_t)7;          /* current solution x_n  */
    seq_t A      = (seq_t)9;          /* A_n = (2*x^2 - x - 1) / 10^n  */
    seq_t pow10  = (seq_t)10;         /* 10^n  */

    for (idx_t k = 1; k < n; ++k) {   /* we already have 1 digit, add k from 1 to n-1 */
        seq_t old_x = x;              /* x_k for this stage */

        /* Compute c = (4*old_x - 1) mod 10. Only last digit of old_x matters. */
        unsigned long long x_last = (unsigned long long)(old_x % (seq_t)10);
        unsigned long long c = (4 * x_last + 9) % 10;   /* +9 == -1 mod 10 */

        /* Inverse of c modulo 10 (c ∈ {1,3,7,9}). */
        unsigned long long inv_c;
        switch ((int)c) {
            case 1: inv_c = 1; break;
            case 3: inv_c = 7; break;
            case 7: inv_c = 3; break;
            case 9: inv_c = 9; break;
            default: inv_c = 0;   /* never occurs */
        }

        /* d = -(A_n) * inv_c  (mod 10) . */
        unsigned long long A_mod10 = (unsigned long long)(A % (seq_t)10);
        unsigned long long d = ((10 - A_mod10) % 10 * inv_c) % 10;
        seq_t d_seq = (seq_t)d;

        /* Update the solution x_{k+1}. */
        x = d_seq * pow10 + old_x;

        /* Update A_{k+1} using A_{k+1} = (A_k + (4*d*old_x - d) + 2*d^2*10^k) / 10. */
        seq_t term1 = A + (4 * d_seq * old_x - d_seq);
        seq_t term2 = 2 * d_seq * d_seq * pow10;
        A = (term1 + term2) / (seq_t)10;

        /* Advance 10^k. */
        pow10 = pow10 * (seq_t)10;
    }

    return x;
}
