/*
 * A110243  Row sums of a Jacobi triangle.
 * https://oeis.org/A110243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=0..n} Jacobi(n, 2n-2k+1)
 * Range:     i8 n <= 64, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = sum_{m odd, 1 ≤ m ≤ 2n+1} Jacobi(n,m) */

#include <stdint.h>

/* Jacobi symbol (a,b) for odd, positive b.
   Returns 1, -1, or 0 (the latter only when a≡0 mod b, b>1). */
static inline seq_t jacobi_symbol(seq_t a, seq_t b)
{
    if ((b & 1) == 0) return 0;          /* b must be odd; fails fast   */
    a = a % b;
    seq_t result = 1;

    while (a != 0) {
        while ((a & 1) == 0) {            /* handle powers of 2 in a   */
            a >>= 1;
            seq_t b_mod8 = b & 7;
            if (b_mod8 == 3 || b_mod8 == 5)
                result = -result;
        }

        /* swap a and b */
        seq_t temp = a;
        a = b;
        b = temp;

        if ((a & 3) == 3 && (b & 3) == 3)
            result = -result;

        a %= b;
    }

    return (b == 1) ? result : 0;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = 2 * n + 1;
    for (idx_t m = 1; m <= limit; m += 2) {
        sum += jacobi_symbol((seq_t)n, (seq_t)m);
    }
    return sum;
}
