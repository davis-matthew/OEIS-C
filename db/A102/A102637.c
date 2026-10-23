/*
 * A102637  Interleaved reading of A000578(n), A011379(n) and A027444(n).
 * https://oeis.org/A102637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Interleaved reading of A000578 (cubes), A011379 (n³+n² except
 *            first term), * and A027444 (n³+n²+n). Special cases: a(1)=1,
 *            a(2)=8. * a(3n) = (n+2)³ * a(3n+1) = 1 for n=0 * (=
 *            (n+1)³+(n+1)²) otherwise * a(3n+2) = n³+n²+n for n≥1 * a(2) = 8
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* offset 1; value undefined for 0 */

    /* Determine position modulo 3 */
    uint64_t r = n % 3;

    if (r == 1) {                 /* a(3k+1) */
        idx_t k = (n + 2) / 3;
        if (k == 1)               /* the exceptional first term */
            return (seq_t)1;
        seq_t kk = (seq_t)k;
        return kk * kk * kk + kk * kk;   /* k³ + k² */
    }
    else if (r == 2) {            /* a(3k+2) */
        if (n == 2)               /* special term a(2)=8 */
            return (seq_t)8;
        idx_t k = (n - 2) / 3;
        seq_t kk = (seq_t)k;
        return kk * kk * kk + kk * kk + kk; /* k³ + k² + k */
    }
    else {                        /* r == 0, a(3k) */
        idx_t k = n / 3 + 2;
        seq_t kk = (seq_t)k;
        return kk * kk * kk;       /* (k)³ where k = n/3 + 2 */
    }
}
