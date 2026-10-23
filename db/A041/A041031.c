/*
 * A041031  Denominators of continued fraction convergents to sqrt(20).
 * https://oeis.org/A041031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued-fraction convergents to sqrt(20).
 * a(0) = 1;  a(1) = 2.
 * For n ≥ 1 let a_k = 2 if k is odd, 8 if k is even.
 * Then a(n) = a_k * a(n-1) + a(n-2), which follows from
 * q_k = a_k q_{k-1} + q_{k-2} for the convergents of sqrt(20). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t q_nm2 = (seq_t)1; /* q_{k-1} for the upcoming iteration (q0) */
    seq_t q_nm1 = (seq_t)0; /* q_{k-2} for the upcoming iteration (q{-1}) */
    seq_t q_n   = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_k = (k & 1ULL) ? (seq_t)2 : (seq_t)8;
        q_n = a_k * q_nm2 + q_nm1;
        q_nm1 = q_nm2;
        q_nm2 = q_n;
    }
    return q_n;
}
