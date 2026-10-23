/*
 * A009411  Expansion of log(1+x)*cosh(log(1+x)).
 * https://oeis.org/A009411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1)/2*(n!*H(n)-(n-2)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires no external libraries; computed with integer arithmetic only. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    idx_t i;
    seq_t fact = (seq_t)1;    /* (i-1)! initially 1! for i=2 */
    seq_t sum  = (seq_t)1;    /* S_1 = 1!*H(1) = 1 */

    for (i = 2; i <= n; ++i) {
        seq_t next_fact = fact * (seq_t)i;   /* i!  */
        sum = sum * (seq_t)i + fact;         /* update S_i = i*S_{i-1} + (i-1)! */
        fact = next_fact;
    }

    /* fact now holds n!; compute (n-2)! = n! / (n*(n-1)) */
    seq_t t = fact / (seq_t)n;
    seq_t fact_nm2 = t / ((seq_t)n - (seq_t)1);

    seq_t diff = sum - fact_nm2;          /* even integer */
    seq_t result = diff / (seq_t)2;       /* divide by 2 */

    /* apply alternating sign: sign = (-1)^(n+1) */
    if ((n & (seq_t)1) == 0) {
        result = -result;                /* n even -> sign = -1 */
    }
    return result;
}
