/*
 * A185509  Fourth accumulation array, T, of the natural number array A000027,
 *          by antidiagonals.
 * https://oeis.org/A185509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of the * n-th
 *            element in the anti‑diagonal traversal of T(n,k). * T(n,k) =
 *            n*(n+1)*(n+2)*(n+3)*k*(k+1)*(k+2)*(k+3) * * (5*n*n + (6*k+39)*n
 *            + 5*k*k + 9*k + 86) / 86400 * (nodename: A185509 – Fourth
 *            accumulation array, T, of A000027.)
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 796, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>


static seq_t compute_T(idx_t n, idx_t k)
{
    /* Convert indices to seq_t to avoid truncation in intermediate
     * calculations. */
    seq_t N = (seq_t)n;
    seq_t K = (seq_t)k;

    /* The numerator factors of T(n,k).  We later cancel 86400 = 2^7*3^3*5^2. */
    seq_t factors[9];
    factors[0] = N;
    factors[1] = N + 1;
    factors[2] = N + 2;
    factors[3] = N + 3;
    factors[4] = K;
    factors[5] = K + 1;
    factors[6] = K + 2;
    factors[7] = K + 3;

    /* Polynomial part */ 
    seq_t poly = 5 * N * N
               + (6 * K + 39) * N
               + 5 * K * K
               + 9 * K
               + 86;
    factors[8] = poly;

    /* Cancel the denominator 86400 = 2^7 * 3^3 * 5^2.
       We maintain an array of the prime factors to remove in
       any order; each  factor can be divided by any prime that
       divides it. */
    static const unsigned primes[] =
    {
        2,2,2,2,2,2,2,       /* seven 2's   */
        3,3,3,               /* three 3's   */
        5,5                  /* two 5's     */
    };
    const size_t primes_cnt = sizeof(primes)/sizeof(primes[0]);

    for (size_t pi = 0; pi < primes_cnt; ++pi)
    {
        unsigned p = primes[pi];
        for (size_t i = 0; i < 9; ++i)
        {
            if (factors[i] % p == 0)
            {
                factors[i] /= p;
                break;
            }
        }
    }

    /* Multiply the remaining integer factors.  The result fits into
       seq_t for all straightforward builds (up to 64 bits). */
    seq_t result = 1;
    for (size_t i = 0; i < 9; ++i)
        result *= factors[i];

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for indices < 1 */

    /* Find the diagonal sum m = i + j (with i,j ≥ 1).  Diagonal m has
       m-1 elements.  We accumulate the total number of terms seen so
       far until we reach the diagonal containing the n-th term. */
    idx_t m = 2;          /* first diagonal (i=1,j=1) has m=2 */
    idx_t seen = 0;       /* terms counted before current diagonal */

    while (seen + (m - 1) < n)
    {
        seen += (m - 1);
        ++m;
    }

    /* Position of the desired term within the current diagonal (0‑based). */
    idx_t offset = n - seen - 1;

    /* Coordinates: i = offset+1, j = m - i. */
    idx_t i = offset + 1;
    idx_t j = m - i;

    return compute_T(i, j);
}
