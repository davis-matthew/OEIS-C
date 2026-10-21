/*
 * A097543  Sum_{k=1..n-1} J(2*n,k)*k^2, where J(i,j) is the Jacobi symbol.
 * https://oeis.org/A097543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1}^{n-1} J(2*n,k) * k^2, where J is the Jacobi
 *            symbol
 * Range:     i8 n <= 11, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Jacobi symbol J(a,n) is defined for odd n>0.
   For even n it is 0. The following implementation follows the
   standard quadratic reciprocity algorithm and works with 64‑bit
   operands.  It returns 1, -1, or 0. */

static inline int jacobi_symbol_uint64(uint64_t a, uint64_t n)
{
    /* Return 0 when n is not odd or is 0. */
    if (n <= 1 || (n & 1) == 0)
        return (n == 1) ? 1 : 0;

    int jac = 1;
    a %= n;

    while (a != 0) {
        /* Handle factors of 2 in a */
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t r = n & 7ULL;            /* n mod 8 */
            if (r == 3 || r == 5)
                jac = -jac;
        }

        /* Swap a and n */
        uint64_t t = a; a = n; n = t;

        /* Quadratic reciprocity sign adjustment */
        if ((a & 3) == 3 && (n & 3) == 3)
            jac = -jac;

        a %= n;
    }

    return (n == 1) ? jac : 0;
}

/* Core OEIS sequence function.  The implementation takes care to
   avoid overflow on 64‑bit indices while performing the calculation
   in the generic seq_t type, which may be arbitrary‑precision. */
seq_t a(idx_t n)
{
    /* Offset starts at 1; the sequence is defined for n >= 1. */
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    /* 2 * n fits in uint64_t for the limits the build tests use
       (n up to about 10 000). */
    uint64_t two_n = n << 1;

    for (idx_t k = 1; k < n; ++k) {
        /* Only odd k contribute; for even k the Jacobi symbol is 0. */
        if ((k & 1) == 0)
            continue;

        int s = jacobi_symbol_uint64(two_n, (uint64_t)k);
        if (s == 0)
            continue;          /* defensive, should not happen for odd k */

        seq_t ksq = (seq_t)k * (seq_t)k;
        if (s > 0)
            sum += ksq;
        else
            sum -= ksq;
    }

    return sum;
}
