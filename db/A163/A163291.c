/*
 * A163291  Number of digits of n-th prime written in base 4.
 * https://oeis.org/A163291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((bitlen_seq_t(get_prime(n))+1)/2)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   i.e. number of base‑4 digits of the n‑th prime.          */

#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.   */
    if (n < 1)
        return 0;                 /* defined behaviour for n<1 is not important */

    /* Get the n‑th prime (as 32‑bit) and cast to seq_t.   */
    seq_t p = (seq_t)get_prime(n);

    /* bitlen_seq_t returns floor(log2(p))+1  (for p>0).  */
    seq_t bits = (seq_t)bitlen_seq_t(p);

    /* Number of base‑4 digits = floor((bits-1)/2)+1 = (bits+1)/2. */
    return (bits + 1) / 2;
}
