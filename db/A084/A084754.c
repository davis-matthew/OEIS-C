/*
 * A084754  Triangle read by rows: row n lists the first n primes greater than
 *          n.
 * https://oeis.org/A084754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = prime(PrimePi(r)+k) where r = ceil((sqrt(8*n+1)-1)/2)
 *            (the row containing a(n)) k = n - r*(r-1)/2 (index within that
 *            row)
 * Range:     u8 n <= 902, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   compute_pi, get_prime, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the row number r */
    idx_t t = int_sqrt(n * 8 + 1);
    idx_t r = (t - 1) / 2;
    while (r * (r + 1) / 2 < n) r++;

    /* Offset within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* Index of the desired prime */
    idx_t pi_r = (idx_t)compute_pi(r);
    idx_t prime_idx = pi_r + k;

    return (seq_t)get_prime(prime_idx);
}
