/*
 * A081232  Let p = n-th prime of the form 4k+1, take smallest solution (x,y)
 *          to the Pellian equation x^2 - p*y^2 = 1 with x and y >= 1;
 *          sequence gives value of x.
 * https://oeis.org/A081232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     
 *  Formulation (OEIS notation):
 *  a(n) = the minimal x ≥ 1 such that x² – p·y² = 1 for y ≥ 1,
 *  where p is the n‑th prime of the form 4k+1.
 *
 *  The function below implements a direct computation:
 *  1. Find the n‑th prime congruent to 1 (mod 4).
 *  2. Solve the Pell equation x² – p·y² = 1 by using the continued‑fraction
 *     expansion of √p and collecting convergents until the equation is
 *     fulfilled.  All arithmetic is performed with type seq_t so that
 *     arbitrary‑precision builds handle large values correctly.
 *
 *  It relies only on standard headers and user‑supplied types
 *  (seq_t, idx_t).  No external libraries or hard‑coded tables are used.
 */

#include <stdbool.h>
#include <stdint.h>

/* --- integer square root for 64‑bit values --------------------------------- */
static unsigned long long sqrt_u64(unsigned long long n)
{
    unsigned long long lo = 0, hi = ~0ULL, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* --- primality test suitable for 64‑bit numbers ----------------------------- */
static bool isPrime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (idx_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- find the n‑th prime congruent to 1 (mod 4) ----------------------------- */
static seq_t prime4k1(idx_t n)
{
    idx_t count = 0;
    idx_t cand  = 5;                     /* first prime of the form 4k+1 is 5 */
    while (true) {
        if ((cand & 3ULL) == 1ULL && isPrime(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        cand += 4;                       /* only numbers of the form 4k+1 */
    }
    /* unreachable, but keeps compiler happy */
    return 0;
}

/* --- minimal solution of x² – D·y² = 1 via continued fractions ------- */
static seq_t minimal_pell_solution(idx_t D)
{
    unsigned long long d0 = sqrt_u64(D);
    if (d0 * d0 == D) return 1;          /* D is a perfect square (should not occur) */

    /* Continued fraction variables */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a = d0;

    /* Convergent recurrence coefficients */
    seq_t h_prevprev = 0; /* h_{-2} */
    seq_t h_prev     = 1; /* h_{-1} */
    seq_t k_prevprev = 1; /* k_{-2} */
    seq_t k_prev     = 0; /* k_{-1} */

    while (true) {
        seq_t h = (seq_t)a * h_prev + h_prevprev;
        seq_t k = (seq_t)a * k_prev + k_prevprev;

        /* Check if h² – D·k² == 1 */
        seq_t lhs = h * h;
        seq_t rhs = (seq_t)D * k * k;
        seq_t diff = lhs - rhs;
        if (diff == 1) return h;

        /* Next terms of the continued fraction */
        unsigned __int128 temp = (unsigned __int128)d * a;
        m = (unsigned long long)(temp - m);

        unsigned __int128 mm  = (unsigned __int128)m * m;
        unsigned __int128 rem = (unsigned __int128)D - mm;
        d = (unsigned long long)(rem / d);

        a = (unsigned long long)((d0 + m) / d);

        /* Advance convergents */
        h_prevprev = h_prev; h_prev = h;
        k_prevprev = k_prev; k_prev = k;
    }
}

/* ------------------------------------------------------------------------- */
/*  Required function for the framework                                     */
seq_t a(idx_t n)
{
    seq_t p = prime4k1(n);
    return minimal_pell_solution((idx_t)p);
}
