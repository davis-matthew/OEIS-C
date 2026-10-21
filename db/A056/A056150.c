/*
 * A056150  Number of combinations for each possible sum when throwing 3
 *          (normal) dice.
 * https://oeis.org/A056150
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of ordered triples (i,j,k) with 1≤i,j,k≤6 and
 *            i+j+k = n
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sums attainable with three six‑sided dice range from 3 to 18.
       For any other n the value is 0. */
    if (n < 3 || n > 18)
        return 0;

    /* Let S = n‑3 be the sum of the non‑negative variables
       x=i−1, y=j−1, z=k−1 (each 0…5).  Count the solutions
       of x+y+z = S with 0 ≤ x,y,z ≤ 5.  A simple double loop
       over x and y suffices because N is fixed to 6. */
    idx_t S = n - 3;          /* 0 ≤ S ≤ 15 */
    seq_t count = 0;

    for (idx_t x = 0; x <= 5; ++x) {
        for (idx_t y = 0; y <= 5; ++y) {
            idx_t sum_xy = x + y;
            if (sum_xy > S)
                continue;                     /* z would be negative */
            idx_t z = S - sum_xy;
            if (z <= 5)
                ++count;                      /* valid triple found */
        }
    }
    return count;
}
