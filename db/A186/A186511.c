/*
 * A186511  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-4+5j^2. Complement
 *          of A186512.
 * https://oeis.org/A186511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((n^2)/5 + 9/10))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor(sqrt((2*n*n + 9)/10)) using 128‑bit arithmetic
       to avoid overflow for large 64‑bit n.  */
    unsigned __int128 nn = n;                     /* cast idx_t to 128‑bit */
    unsigned __int128 x = 2 * nn * nn + 9;        /* numerator */

    unsigned __int128 low = 0, high = nn;        /* k ≤ n for all n ≥ 1 */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        unsigned __int128 cmp = 10 * mid * mid;   /* compare 10*mid^2 ≤ x */
        if (cmp <= x)
            low = mid;
        else
            high = mid - 1;
    }

    seq_t k = (seq_t)low;
    return (seq_t)n + k;
}
