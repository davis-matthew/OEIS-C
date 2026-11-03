/*
 * A188569  Degree of the n-th partition class polynomial Hpart_n(x).
 * https://oeis.org/A188569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced positive definite binary quadratic
 *            forms of discriminant -(24n-1) (i.e., the class number of the
 *            imaginary quadratic order of discriminant -(24n-1))
 * Range:     u8 n <= 750, u16 n <= 750, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Validated: standard, pass, 82 terms through n = 647 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count reduced positive definite binary quadratic forms of discriminant -d,
   where d > 0 and d ≡ 3 (mod 4).  This equals the class number of the
   imaginary quadratic order with discriminant -d. */
static seq_t class_number_for_d(idx_t d)
{
    seq_t count = 0;
    for (idx_t a = 1; a * a * 3 <= d; ++a)
    {
        int64_t a_signed = (int64_t)a;

        /* b has the same parity as D = -d; since d ≡ 3 (mod 4), D ≡ 1 (mod 4)
           so b must be odd.  Start at the smallest odd value ≥ -a. */
        int64_t b_start = -a_signed;
        if ((b_start & 1) == 0)
            b_start += 1;

        /* Enumerate all b with |b| ≤ a, step 2 (keeping parity). */
        for (int64_t b = b_start; b <= a_signed; b += 2)
        {
            /* c = (b² - D)/(4a) = (b² + d)/(4a) */
            idx_t numerator = (idx_t)(b * b) + d;
            if (numerator % (4 * a) != 0)
                continue;
            idx_t c = numerator / (4 * a);

            if (a > c)
                continue;  /* a ≤ c is required for a reduced form */

            /* Reducedness condition: if a == c or |b| == a, then b ≥ 0. */
            if ((a == c || b == -a_signed) && b < 0)
                continue;

            count++;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* safety for offsets < 1 */

    idx_t d = 24 * n - 1;   /* d = 24n-1, always odd ≡ 3 (mod 4) */
    return class_number_for_d(d);
}
