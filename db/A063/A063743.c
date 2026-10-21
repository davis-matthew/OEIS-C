/*
 * A063743  Numbers k such that k and Omega(k) are relatively prime, where
 *          Omega(k) is the number of prime divisors of k (with repetition).
 * https://oeis.org/A063743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A063743: numbers k such that Ω(k) and k are coprime. */
#include "../utils/gcd.h"
#include "../utils/bigomega.h"

/* Return the n-th term of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for other
       indices to keep the function total.  */
    if (n < 1)
        return 0;

    idx_t k = 1;          /* current integer being tested      */
    idx_t count = 0;      /* how many qualifying integers seen */

    /* Simple linear search: test each integer in turn until the
       n‑th qualifying integer is found.  With the density
       6/π² (~0.6079), the n-th term for n = 10 000 is ≈ 16 500,
       so the loop is trivial in time.  */
    while (1) {
        seq_t omega = bigomega(k);            /* Ω(k)  (count with multiplicity) */
        if (gcd((seq_t)k, omega) == 1) {     /* gcd(k, Ω(k)) == 1 ?   */
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
