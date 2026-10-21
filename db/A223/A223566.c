/*
 * A223566  Number of nX5 0..1 arrays with antidiagonals unimodal.
 * https://oeis.org/A223566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π{ (1 + (i*(i+1)/2))^2 , i = 1..(min(n,5)-1) } * (1 +
 *            (min(n,5)*(min(n,5)+1)/2))^(max(n,5)-min(n,5)+1) Recurrence for
 *            n > 4: a(n) = 16 * a(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; for n == 0 we return 0 */
    if (n == 0) return (seq_t)0;

    idx_t min_n5 = (n < 5 ? n : 5);

    /* product of the squared terms for i = 1 .. min_n5-1 */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < min_n5; ++i) {
        seq_t tri  = (seq_t)i * ((seq_t)i + (seq_t)1) / (seq_t)2;
        seq_t term = (seq_t)1 + tri;
        result *= term * term;
    }

    /* last factor and its exponent */
    seq_t tri_last = (seq_t)min_n5 * ((seq_t)min_n5 + (seq_t)1) / (seq_t)2;
    seq_t last      = (seq_t)1 + tri_last;
    int   exp_int;
    if (n >= 5)
        exp_int = (int)(n - 4);          /* exponent = n-4 for n ≥ 5 */
    else
        exp_int = (int)(6 - (int)n);     /* exponent = 6-n for n < 5 */

    /* multiply the result by last^exp_int */
    for (int k = 0; k < exp_int; ++k)
        result *= last;

    return result;
}
