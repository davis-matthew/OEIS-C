/*
 * A211315  Square array of Delannoy numbers D(i,j) mod 11 (i >= 0, j >= 0)
 *          read by antidiagonals.
 * https://oeis.org/A211315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = D(i,j) mod 11 where D(i,j) is the Delannoy number and
 *            (i,j) is obtained by the antidiagonal reading of the square
 *            array. i+j = ⌊(√(8n+1)−1)/2⌋ and i = n − (i+j)(i+j+1)/2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* small_binom[n][k] = C(n,k) mod 11 for 0 ≤ n,k ≤ 10 */
static unsigned char small_binom[11][11];
static int small_binom_init = 0;

static void init_small_binom(void)
{
    if (small_binom_init) return;
    for (int n = 0; n <= 10; ++n) {
        small_binom[n][0] = small_binom[n][n] = 1 % 11;
        for (int k = 1; k < n; ++k) {
            small_binom[n][k] = (small_binom[n-1][k-1] + small_binom[n-1][k]) % 11;
        }
    }
    small_binom_init = 1;
}

/* binom_mod_11(n,k) uses Lucas theorem for modulus 11 */
static unsigned char binom_mod_11(idx_t n, idx_t k)
{
    init_small_binom();
    unsigned char res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 11;
        int ki = k % 11;
        if (ki > ni) return 0;
        res = (unsigned char)((res * small_binom[ni][ki]) % 11);
        n /= 11;
        k /= 11;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* determine the antidiagonal index d = i + j */
    long double tmp = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t d = (idx_t)((tmp - 1.0L) / 2.0L);

    /* triangular number of previous diagonal: T(d-1) = d*(d+1)/2 */
    idx_t t_prev = d * (d + 1) / 2;

    idx_t i = n - t_prev;
    idx_t j = d - i;

    /* compute D(i,j) mod 11 */
    idx_t min_ij = (i < j) ? i : j;
    unsigned int power2 = 1;   /* 2^k mod 11 */
    unsigned char acc = 0;     /* accumulator modulo 11 */

    for (idx_t k = 0; k <= min_ij; ++k) {
        unsigned char b1 = binom_mod_11(i, k);
        unsigned char b2 = binom_mod_11(j, k);
        unsigned int term = (unsigned int)b1 * b2 % 11;
        term = (unsigned int)(term * power2 % 11);
        acc += term;
        acc %= 11;
        power2 = (unsigned int)(power2 * 2 % 11);
    }

    return (seq_t)acc;
}
