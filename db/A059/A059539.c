/*
 * A059539  Beatty sequence for 3^(1/3).
 * https://oeis.org/A059539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * cbrt(3)) (Beatty sequence for 3^(1/3))
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1288, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term shown in the OEIS entry */
    if (n == 0) return 0;

    /* Compute y = 3 * n^3 as a seq_t (big integer if needed) */
    seq_t nn = (seq_t)n;
    seq_t y  = nn * nn * nn * (seq_t)3;

    /* Binary search for the largest integer m with m^3 <= y
       while m <= 2*n is a safe upper bound because (2n)^3 = 8*n^3 >= 3*n^3 */
    seq_t low  = (seq_t)0;
    seq_t high = (seq_t)2 * nn;          /* upper bound for the answer */

    while (low < high) {
        seq_t mid = (low + high + (seq_t)1) >> 1;  /* mid = ceil((low+high)/2) */
        seq_t cube = mid * mid * mid;
        if (cube <= y)
            low = mid;
        else
            high = mid - (seq_t)1;
    }

    /* low equals floor( n * cbrt(3) ) */
    return low;
}
