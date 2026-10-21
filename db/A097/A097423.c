/*
 * A097423  Numerator of Product_{k=1..n} H(k), where H(k) = Sum_{j=1..k} 1/j,
 *          the k-th harmonic number.
 * https://oeis.org/A097423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∏_{k=1..n} H(k), where H(k)=∑_{j=1..k} 1/j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented with on-the-fly reduction to keep intermediate values as small as possible.
   An auxiliary Euclidean gcd routine is supplied locally to avoid relying on external headers. */

static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t H_num = (seq_t)1;   /* numerator of current H(k)   */
    seq_t H_den = (seq_t)1;   /* denominator of current H(k) */

    seq_t P_num = (seq_t)1;   /* numerator of running product   */
    seq_t P_den = (seq_t)1;   /* denominator of running product */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* update H(k) */
        if (k == (idx_t)1)
        {
            H_num = (seq_t)1;
            H_den = (seq_t)1;
        }
        else
        {
            seq_t k_seq = (seq_t)k;
            seq_t new_num = H_num * k_seq + H_den;
            seq_t new_den = H_den * k_seq;
            seq_t gH = my_gcd(new_num, new_den);
            H_num = new_num / gH;
            H_den = new_den / gH;
        }

        /* multiply H(k) into the running product P = P_num / P_den */
        seq_t g1 = my_gcd(P_num, H_den);
        P_num = P_num / g1;
        seq_t den1 = H_den / g1;

        seq_t g2 = my_gcd(H_num, P_den);
        P_den = P_den / g2;
        seq_t num2 = H_num / g2;

        P_num = P_num * num2;
        P_den = P_den * den1;

        seq_t g3 = my_gcd(P_num, P_den);
        P_num = P_num / g3;
        P_den = P_den / g3;
    }

    return P_num;   /* numerator of the reduced product */
}
