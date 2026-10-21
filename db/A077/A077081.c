/*
 * A077081  Fixed point when phi(sigma(n)+phi(n))=A077080 is iterated with
 *          initial value of n.
 * https://oeis.org/A077081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A077081: a(n) = fixed point of the iteration  
 *               x -> phi( sigma(x) + phi(x) ).  
 * The sequence is computed by repeatedly applying the
 * A077080 rule until a fixed point is reached.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* start value */

    while (1) {
        /* convert to idx_t so we can call the utility functions */
        idx_t xi = (idx_t)x;

        seq_t phi_x   = phi(xi);               /* φ(x)   */
        seq_t sigma_x = sigma_of(xi);          /* σ(x)   */

        seq_t sum = phi_x + sigma_x;           /* σ(x)+φ(x)  */
        seq_t y   = phi((idx_t)sum);           /* φ(σ(x)+φ(x)) */

        if (y == x)           /* fixed point reached */
            return x;

        x = y;                /* continue iteration */
    }
}
