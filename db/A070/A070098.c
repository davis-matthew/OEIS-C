/*
 * A070098  Number of integer triangles with perimeter n which are acute and
 *          isosceles.
 * https://oeis.org/A070098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2) - floor(n/(2 + sqrt(2))) - ((n + 1) mod 2) *
 *            Computation: enumerate all integer isosceles triangles with
 *            perimeter n * and count those that are acute. For a triangle
 *            with sides (a,a,b) * the triangle inequalities become 4a > n and
 *            0 < b = n - 2a. * The triangle is acute iff 2a²...
 * Range:     u8 n <= 29, u16 n <= 1302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 there is no integer triangle with perimeter n */
    if (n < 3)
        return (seq_t)0;

    /* Minimal a that satisfies 4a > n  →  a > n/4  */
    idx_t a_min = n / 4 + 1;

    /* Maximal a that keeps b > 0  →  a < n/2  */
    idx_t a_max = (n - 1) / 2;

    seq_t count = 0;

    for (idx_t a = a_min; a <= a_max; ++a) {
        idx_t b = n - 2 * a;          /* base side */
        seq_t left  = (seq_t)a * (seq_t)a;
        left += left;                  /* 2 * a² */
        seq_t right = (seq_t)b * (seq_t)b;
        if (left > right)
            ++count;
    }

    return count;
}
