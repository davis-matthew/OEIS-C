/*
 * A176353  A symmetrical triangle sequence based on Dirichlet's
 *          divisors:g(n)=n*Log[n] - n + Sqrt[n];t(n,m)=If[m == 0 || m == n,
 *          1, 1 + Round[ -g(m) - g(n - m) + g(n)]].
 * https://oeis.org/A176353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176353: 
   a(n) = t(r,m)   where r = floor((sqrt(8n+1)-1)/2) and m = n - r*(r+1)/2
   t(r,m) = 1 if m==0 or m==r; otherwise
            1 + round(-g(m)-g(r-m)+g(r))
   g(k) = k*log(k) - k + sqrt(k)  (natural log) */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Find the row r and column m of the flattened triangle */
    /* r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    /* Use double for the estimate and correct by a few steps */
    double d = sqrt((double)8.0 * n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* Correct possible rounding errors */
    while (( (__int128) r * (r + 1) / 2 ) > n) r--;
    while (( (__int128) (r + 1) * (r + 2) / 2 ) <= n) r++;

    idx_t offset = (idx_t)(( (__int128) r * (r + 1) ) / 2);
    idx_t m = n - offset;

    /* Handle the border values */
    if (m == 0 || m == r) {
        return (seq_t)1;
    }

    /* Compute the g values using double precision */
    double rd = (double)r;
    double md = (double)m;
    double rmd = rd - md;   /* r - m = n - m? actually r-m = n-m when r = n; but here r is row, safe */
    /* Note rmd equals r - m as integers */
    double g_r = rd * log(rd) - rd + sqrt(rd);
    double g_m = md * log(md) - md + sqrt(md);
    double g_rm = rmd * log(rmd) - rmd + sqrt(rmd);

    double E = -g_m - g_rm + g_r;        /* exact expression inside round   */
    int s = (int)floor(E + 0.5);          /* round to nearest integer */
    int t = 1 + s;                       /* final triangle entry */

    return (seq_t)t;
}
