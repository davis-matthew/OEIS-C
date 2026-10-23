/*
 * A119947  Triangle of numerators in the square of the matrix A[i,j] = 1/i
 *          for j <= i, 0 otherwise.
 * https://oeis.org/A119947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = numerator( r(i,j) ) where r(i,j) = (1/i)*sum_{k=j..i} 1/k
 *            for the flat index n (1‑based). Uses the formula r(i,j) = ( H_i
 *            - H_{j-1} ) / i, where H_m = sum_{k=1..m} 1/k in lowest terms.
 *            Implementation keeps a memoised table of harmonic numerators and
 *            denominators
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* external types supplied by the build */
extern seq_t a(idx_t n);

/* static cache for harmonic numbers */
static seq_t *h_num = NULL;  /* numerators H_m */
static seq_t *h_den = NULL;  /* denominators H_m */
static idx_t h_max = 0;      /* highest m for which H_m is stored */

/* ensure that harmonic numbers up to m are computed */
static void ensure_harmonic(idx_t m)
{
    if (m <= h_max) return;

    /* allocate or grow arrays */
    if (h_max == 0) {
        h_num = (seq_t *)malloc( (size_t)(m+1) * sizeof(seq_t) );
        h_den = (seq_t *)malloc( (size_t)(m+1) * sizeof(seq_t) );
    } else if (m > h_max) {
        h_num = (seq_t *)realloc( h_num, (size_t)(m+1) * sizeof(seq_t) );
        h_den = (seq_t *)realloc( h_den, (size_t)(m+1) * sizeof(seq_t) );
    }
    if (!h_num || !h_den) return;   /* allocation failure → leave cache empty */

    /* initialise H_0 = 0/1 */
    h_num[0] = 0;
    h_den[0] = 1;

    /* compute successive harmonics */
    for (idx_t k = h_max + 1; k <= m; ++k) {
        /* H_{k-1} = p/q, add 1/k => (p*k + q) / (q*k), then reduce */
        seq_t pk = h_num[k-1];
        seq_t qk = h_den[k-1];
        seq_t num = pk * (seq_t)k + qk;
        seq_t den = qk * (seq_t)k;
        seq_t g   = gcd( num, den );
        h_num[k]  = num / g;
        h_den[k]  = den / g;
    }
    h_max = m;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* find row i such that T(i-1) < n <= T(i),  T(i)=i*(i+1)/2 */
    idx_t i = 1;
    while ( (i*(i+1))/2 < n ) ++i;

    /* column j (1‑based) */
    idx_t prev_tri = (i-1)*i/2;
    idx_t j = n - prev_tri;        /* j in [1,i] */

    /* ensure harmonic numbers up to i are ready */
    ensure_harmonic( i );

    if (!h_num || !h_den) return 0; /* allocation failure */

    /* H_i = h_num[i]/h_den[i]   ,   H_{j-1} = h_num[j-1]/h_den[j-1] */
    seq_t p_i = h_num[i];
    seq_t q_i = h_den[i];
    seq_t p_j = h_num[j-1];
    seq_t q_j = h_den[j-1];

    /* S = H_i - H_{j-1} = (p_i*q_j - p_j*q_i) / (q_i*q_j) */
    seq_t numS   = p_i * q_j - p_j * q_i;
    seq_t denomS = q_i * q_j;
    seq_t gS     = gcd( numS, denomS );
    numS /= gS;
    denomS /= gS;

    /* r(i,j) = numS / (denomS * i) */
    seq_t denomR = denomS * (seq_t)i;
    seq_t gR     = gcd( numS, denomR );
    seq_t result = numS / gR;

    return result;
}
