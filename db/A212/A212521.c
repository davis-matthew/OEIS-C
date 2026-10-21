/*
 * A212521  Number of (w,x,y,z) with all terms in {1,...,n} and w>=2x and
 *          y<=3z.
 * https://oeis.org/A212521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 378, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212521: Count of tuples (w,x,y,z) with 1≤w,x,y,z≤n, w≥2x and y≤3z.
 * Let
 *   f(n) = floor(n/2) * (n - floor(n/2))          // # of (x,w) with w≥2x
 *   g(n) = 3*floor(n/3)*(floor(n/3)+1)/2
 *           + (n - floor(n/3)) * n                // # of (z,y) with y≤3z
 * Then a(n) = f(n) * g(n).
 */
seq_t a(idx_t n)
{
    idx_t m = n / 2;                           // floor(n/2)
    seq_t f = (seq_t)m * (seq_t)(n - m);       // f(n)

    idx_t k = n / 3;                           // floor(n/3)
    seq_t g_part1 = (seq_t)3 * (seq_t)k * (seq_t)(k + 1);
    g_part1 /= 2;                              // 3*k*(k+1)/2

    seq_t g = g_part1 + ((seq_t)n - (seq_t)k) * (seq_t)n; // g(n)

    return f * g;                             // a(n)
}
