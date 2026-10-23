/*
 * A189676  a(n) = n + [nr/s] + [nt/s]; r=Pi/2, s=arcsin(3/5), t=arcsin(4/5).
 * https://oeis.org/A189676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * floor( n * (π/2) / arcsin(3/5) )
 * Range:     u8 n <= 52, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Pre‑compute the constant α = (π/2) / arcsin(3/5).  */
static const long double ALPHA = (acosl((long double)-1) / 2.0L) / asinl((long double)3/5.0L);

seq_t a(idx_t n)
{
    /* Compute n * α in long double, take the floor, then multiply by 2. */
    long double prod = ALPHA * (long double)n;
    long long k = (long long)floorl(prod);  /* floor( n * α )  */
    return (seq_t)(2 * k);
}
