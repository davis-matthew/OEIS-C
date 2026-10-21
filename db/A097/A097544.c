/*
 * A097544  Sum_{k=1..2n-1} J(4*n,k)*k^2, where J(i,j) is the Jacobi symbol.
 * https://oeis.org/A097544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..2n-1} J(4*n,k) * k^2 (Jacobi symbol)
 * Range:     i8 n <= 5, i16 n <= 35, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Jacobi symbol (a/n) for odd positive integer n.
   If n is even, the symbol is defined to be 0.  The routine
   follows the standard binary algorithm. */
static inline int jacobi_symbol(uint64_t a, uint64_t n)
{
    if (n == 0) return 0;          /* undefined */
    if ((n & 1ULL) == 0) return 0; /* even denominator → 0 */

    int s = 1;                     /* sign accumulator */
    a %= n;                        /* reduce numerator */

    while (a != 0) {
        while ((a & 1ULL) == 0) {   /* factor powers of 2 from a */
            a >>= 1;
            switch (n & 7ULL) {     /* n mod 8 */
                case 3: case 5: s = -s; break; /* (2/n) = -1 */
            }
        }
        /* swap a and n */
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3ULL) == 3 && (n & 3ULL) == 3) s = -s; /* reciprocity */

        a %= n;                     /* reduce numerator again */
    }
    return (n == 1) ? s : 0;        /* n reduced to 1 → result s */
}

/* Sequence a(n).  idx_t and seq_t are supplied by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against 0 */

    seq_t sum = 0;
    uint64_t A4 = 4ULL * (uint64_t)n;   /* 4*n fits in 64‑bit for n≤1e6 */
    idx_t limit = 2 * n - 1;              /* last k to sum */

    /* Only odd k can contribute, because J(4*n,k)=0 when k is even. */
    for (idx_t k = 1; k <= limit; k += 2) {
        int sym = jacobi_symbol(A4, (uint64_t)k);
        if (sym != 0) {
            seq_t kk = (seq_t)k * (seq_t)k;   /* k² in seq_t */
            sum += (seq_t)sym * kk;
        }
    }
    return sum;
}
