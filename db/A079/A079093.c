/*
 * A079093  Squarefree kernel of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(((p_n + 1)*(p_{n+1} + 1))/4) where p_k is the k‑th
 *            prime
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 6563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the squarefree kernel (radical) of (prime(n)+1)*(prime(n+1)+1)/4. */

#include "../utils/get_prime.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  No special handling for n < 1 is required in the test suite. */
    uint32_t p1 = get_prime(n);       /* p_n */
    uint32_t p2 = get_prime(n + 1);   /* p_{n+1} */
    seq_t val = ((seq_t)(p1 + 1)) * ((seq_t)(p2 + 1)); /* (p_n+1)*(p_{n+1}+1) */
    val = val / 4;                    /* divide by 4 */
    return rad((idx_t)val);           /* squarefree kernel of the result */
}
