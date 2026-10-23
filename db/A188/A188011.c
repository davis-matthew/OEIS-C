/*
 * A188011  a(n) = floor(n*r) - floor(n*r-k*r) - floor(k*r), where
 *          r=(1+sqrt(5))/2, k=3.
 * https://oeis.org/A188011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi) - floor((n-3)*phi) - 4
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = (1+sqrt(5))/2  */

#include "../utils/floor_sqrt.h"

static int64_t floor_phi_int64(int64_t n)
{
    if (n >= 0) {
        seq_t nn = (seq_t)n;
        seq_t sq  = floor_sqrt((seq_t)5 * nn * nn);   /* floor(n*sqrt(5)) */
        seq_t res = (nn + sq) / 2;                   /* floor(n*phi) */
        return (int64_t)res;
    } else {
        int64_t p   = -n;
        seq_t pp    = (seq_t)p;
        seq_t sq    = floor_sqrt((seq_t)5 * pp * pp); /* floor(p*sqrt(5)) */
        seq_t respos= (pp + sq) / 2;                  /* floor(p*phi) */
        int64_t res = -(int64_t)respos - 1;           /* floor(n*phi) for n<0 */
        return res;
    }
}

seq_t a(idx_t n)
{
    int64_t f1 = floor_phi_int64((int64_t)n);          /* floor(n*phi)       */
    int64_t f2 = floor_phi_int64((int64_t)n - 3);      /* floor((n-3)*phi)   */
    int64_t rv = f1 - f2 - 4;                         /* a(n)                */
    return (seq_t)rv;                                 /* non‑negative value  */
}
