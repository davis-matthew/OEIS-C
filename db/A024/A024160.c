/*
 * A024160  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that (c-b)(c-a)(b-a) divides (c+b)(c+a)(b+a).
 * https://oeis.org/A024160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles a<b<c with a+b+c=n such
 *            that * (c-b)(c-a)(b-a) divides (c+b)(c+a)(b+a). The function
 *            enumerates all admissible * triples (a,b,c) for a given n
 * Range:     u8 n <= 10, u16 n <= 60, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    if (n < (idx_t)9)         /* no triangles for n < 9 */
        return count;

    seq_t n_seq = (seq_t)n;

    /* Range of c:  ceil((n+3)/3) .. floor((n-1)/2)  */
    idx_t c_min = (n + (idx_t)5) / (idx_t)3;   /* ceil((n+3)/3) = (n+5)/3 */
    idx_t c_max = (n - (idx_t)1) / (idx_t)2;   /* floor((n-1)/2) */

    for (idx_t c = c_min; c <= c_max; ++c)
    {
        seq_t s = n_seq - (seq_t)c;          /* a + b */
        if (s <= (seq_t)1) continue;         /* need at least a=1,b=1 */

        /* a must satisfy:  a > s - c   (since b = s - a < c) */
        /* and    a <= floor((s-1)/2)   (since a < b) */
        seq_t a_low_seq  = (s > (seq_t)c) ? (s - (seq_t)c + (seq_t)1) : (seq_t)1;
        idx_t a_low   = (idx_t)a_low_seq;

        seq_t half_s_minus1 = (s - (seq_t)1) / (seq_t)2;
        idx_t a_high  = (idx_t)half_s_minus1;

        if (a_low > a_high) continue;

        for (idx_t a = a_low; a <= a_high; ++a)
        {
            seq_t a_seq = (seq_t)a;
            seq_t b_seq = s - a_seq;          /* b > a by construction */

            /* differences */
            seq_t c_minus_b = (seq_t)c - b_seq;
            seq_t c_minus_a = (seq_t)c - a_seq;
            seq_t b_minus_a = b_seq - a_seq;

            /* numerator: product of differences */
            seq_t numerator = c_minus_b * c_minus_a * b_minus_a;

            /* sums */
            seq_t c_plus_b = (seq_t)c + b_seq;
            seq_t c_plus_a = (seq_t)c + a_seq;
            seq_t b_plus_a = b_seq + a_seq;

            /* denominator: product of sums */
            seq_t denominator = c_plus_b * c_plus_a * b_plus_a;

            if (numerator == (seq_t)0)
                continue;                         /* should not happen */

            if (denominator % numerator == (seq_t)0)
                count += (seq_t)1;
        }
    }
    return count;
}
