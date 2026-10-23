/*
 * A103193  A sequence of triples arising from a matrix calculation, in
 *          particular let m = floor(n/3), then (a(3*m), a(3*m+1), a(3*m+2)) =
 *          M^(m*(m+1)/n) * (0, 1, 1) where M is the matrix [[2,0,1], [0,1,0],
 *          [-2,1,0]].
 * https://oeis.org/A103193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) is given by the matrix power Let m = floor(n/3) and e =
 *            m*(m+1)/2. Compute v = M^e * (0,1,1) where M = [[2,0,1],
 *            [0,1,0], [-2,1,0]]. Return v[ n mod 3 ]
 * Range:     i8 n <= 14, i16 n <= 22, i32 n <= 32, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   mat_mul, mat_vec_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mat_mul.h"
#include "../utils/mat_vec_mul.h"

seq_t a(idx_t n)
{
    /* Base matrix M. */
    static const seq_t M[3][3] = {
        { (seq_t)2, (seq_t)0, (seq_t)1 },
        { (seq_t)0, (seq_t)1, (seq_t)0 },
        { (seq_t)-2, (seq_t)1, (seq_t)0 }
    };

    /* Compute m = floor(n/3) and exponent e = m*(m+1)/2. */
    idx_t m = n / 3;
    idx_t e = m * (m + 1) / 2;

    /* Matrix exponentiation: result = M^e. */
    seq_t result[3][3] = { { (seq_t)1, (seq_t)0, (seq_t)0 },
                           { (seq_t)0, (seq_t)1, (seq_t)0 },
                           { (seq_t)0, (seq_t)0, (seq_t)1 } };   /* identity */
    seq_t base[3][3];
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            base[i][j] = M[i][j];

    while (e > 0)
    {
        if (e & 1)
            mat_mul(result, base, result);
        mat_mul(base, base, base);
        e >>= 1;
    }

    /* Multiply by vector (0,1,1). */
    const seq_t init_vec[3] = { (seq_t)0, (seq_t)1, (seq_t)1 };
    seq_t prod[3];
    mat_vec_mul(result, init_vec, prod);

    /* Return the component corresponding to n mod 3. */
    return prod[ n % 3 ];
}
