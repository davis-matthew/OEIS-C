/*
 * A141658  A triangle of coefficients representing an Integer Irrational
 *          rotation of the primes: t(n,m)=Floor[Re[Exp[I*2*Pi*Mod[m*2*Pi,
 *          1]/(n + 2)]*Prime[n + 1]]]. ( Correction with right numbers).
 * https://oeis.org/A141658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   t(n,m) = Floor[Re[Exp[I*2*Pi*Mod[m*2*Pi, 1]/(n + 2)]*Prime[n +
 *            1]]] a(n) is the flattened triangle: a(n) = t(r-1, m-1) where r
 *            is the row containing index n and m its column index (both
 *            1‑based)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Natural numbers start at 1. n < 1 is not used. */
    idx_t row = 0;
    idx_t len = 1;           /* cumulative number of elements up to current row */
    /* Find the row that contains the index n. */
    while (n > len) {
        row++;
        len += (row + 1);    /* each new row adds row+1 elements */
    }

    /* Elements before this row */
    idx_t before = row * (row + 1) / 2;
    idx_t idx_in_row = n - before - 1;   /* 0‑based column index within the row */

    /* Retrieve the required prime.  get_prime is 1‑based. */
    uint32_t prime = get_prime(row + 1);

    /* Compute the angle for the cosine factor. */
    const double PI = acos(-1.0);
    double fractional = fmod((double)idx_in_row * 2.0 * PI, 1.0);      /* Mod[m*2π,1] */
    double angle = 2.0 * PI * fractional / (double)(row + 2);         /* /(n+2) */

    /* Evaluate floor[prime * cos(angle)] */
    double prod = prime * cos(angle);
    long long val = (long long)floor(prod);

    return (seq_t)val;
}
