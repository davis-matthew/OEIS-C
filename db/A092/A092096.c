/*
 * A092096  a(n) = Sum_{i=0,1,2,..; n-k*i >= -n} |n-k*i| for k=5.
 * https://oeis.org/A092096
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor(2n/5)} |n - 5i|
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute integer parts needed for the closed‑form */
    idx_t q = n / 5;                 /* floor(n/5)           */
    idx_t r = n % 5;                 /* n mod 5              */
    idx_t s = (2 * r) / 5;           /* floor(2*r/5)         */

    /* cast to seq_t for all arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t q_seq = (seq_t)q;
    seq_t s_seq = (seq_t)s;
    const seq_t five = (seq_t)5;

    /* Sum of terms where n-5*i >= 0   (i = 0 .. q) */
    seq_t sum_pos = (q_seq + (seq_t)1) * n_seq
                    - five * q_seq * (q_seq + (seq_t)1) / (seq_t)2;

    /* Count of negative terms   (i = q+1 .. 2q+s) */
    seq_t neg_terms = q_seq + s_seq;

    /* Sum of indices i from q+1 to 2q+s   (used for the negative part) */
    seq_t two_q_plus_s = (seq_t)2 * q_seq + s_seq;
    seq_t sum_i = ((two_q_plus_s * (two_q_plus_s + (seq_t)1))
                   - (q_seq * (q_seq + (seq_t)1))) / (seq_t)2;

    /* Sum of negative terms   */
    seq_t sum_neg = five * sum_i - neg_terms * n_seq;

    return sum_pos + sum_neg;
}
