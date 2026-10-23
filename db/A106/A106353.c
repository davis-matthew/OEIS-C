/*
 * A106353  Number of compositions of n into 4 parts such that no two adjacent
 *          parts are equal.
 * https://oeis.org/A106353
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1 choose 3) - 3*floor((n-2)/2)*(n - floor((n-2)/2) -
 *            2) * + 2*floor((n-1)/3) + (n even ? n/2 - 1 : 0) * - (n mod 4 ==
 *            0 ? 1 : 0) * for all n >= 6. a(n) counts 4‑part compositions of
 *            n with no equal * adjacent parts
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;            /* below offset – undefined */

    /* total compositions into 4 positive parts: C(n-1,3) */
    seq_t total = (seq_t)(n - 1) * (n - 2) * (n - 3) / 6;

    /* first‑order violations (a=b, b=c, or c=d) */
    idx_t m  = (n - 2) / 2;                 /* floor((n-2)/2)           */
    seq_t ev1ev2ev3 = (seq_t)3 * m * (n - m - 2);

    /* pair intersections */
    idx_t k1 = (n - 1) / 3;                       /* floor((n-1)/3)      */
    idx_t k2 = (n % 2 == 0) ? (n / 2 - 1) : 0;    /* n even?            */
    seq_t pairSum = (seq_t)2 * k1 + k2;

    /* triple intersection */
    seq_t triple = (n % 4 == 0) ? 1 : 0;

    seq_t result = total - ev1ev2ev3 + pairSum - triple;
    return result;
}
