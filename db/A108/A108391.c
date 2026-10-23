/*
 * A108391  Expansion of (1 - x + 3*x^2 + 4*x^4 + 8*x^5 + 3*x^6 + x^7 + x^8) /
 *          ((1 + x)*(1 - x + x^2)*(1 - x - x^2)*(1 + x + 2*x^2 - x^3 + x^4)).
 * https://oeis.org/A108391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-3) + 5*a(n-6) + a(n-9) for n > 4 initial values:
 *            a(0) = 1 a(1) = -1 a(2) = 3 a(3) = 3 a(4) = 1 The sequence can
 *            be expressed in blocks of three: a(3k) = b(k) a(3k+1) =
 *            (-1)^(k+1) a(3k+2) = b(k+1) where b(0)=1, b(1)=3, b(2)=17,
 *            b(3)=67, b(4)=289, and for k ≥ 5: b(k) = 3*b(k-1) + 5*b(k...
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 43, i64 n <= 91, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the block index and position within the block */
    idx_t k = n / 3;
    idx_t r = n % 3;

    if (r == 1) {
        /* a(3k+1) = (-1)^(k+1): negative for even k, positive for odd k */
        return ( (k & 1ULL) ? (seq_t)1 : (seq_t)-1 );
    } else {
        /* Need the value b(m) where m = k for r == 0,
           and m = k+1 for r == 2 */
        idx_t m = (r == 0) ? k : (k + 1);

        /* Compute b(m) iteratively */
        if (m == 0) return (seq_t)1;
        if (m == 1) return (seq_t)3;
        if (m == 2) return (seq_t)17;

        seq_t a0 = 1;   /* b(k-3) */
        seq_t a1 = 3;   /* b(k-2) */
        seq_t a2 = 17;  /* b(k-1) */
        seq_t cur;

        for (idx_t i = 3; i <= m; ++i) {
            cur = 3 * a2 + 5 * a1 + a0;  /* b(i) = 3*b(i-1) + 5*b(i-2) + b(i-3) */
            a0 = a1;
            a1 = a2;
            a2 = cur;
        }
        return a2;
    }
}
