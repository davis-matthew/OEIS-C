/*
 * A228529  a(n) = prime(n*prime(n)).
 * https://oeis.org/A228529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n*prime(n))
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

/* Return the k-th prime (k >= 1).  Uses binary search over the prime counting
   function compute_pi.  Limits: k <= 10^9 works within 64‑bit indices. */
static seq_t nth_prime(idx_t k)
{
    /* Handle the first few primes by a small table. */
    if (k <= 6) {
        static const seq_t sp[6] = {2,3,5,7,11,13};
        return sp[k-1];
    }

    idx_t low  = k + 1;          /* lower bound for the k-th prime */
    idx_t high = k * k;          /* trivial upper bound (k^2) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cnt = compute_pi(mid);
        if (cnt >= (seq_t)k) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; by definition a(1)=primer(1*primer(1)) = 3. */
    if (n == 0) return 0;  /* undefined for n=0, harmless for tests */

    /* First, get the n‑th prime.  get_prime returns a uint32_t, which
       comfortably fits into idx_t for n up to 10^4. */
    idx_t p_n = (idx_t)get_prime(n);

    /* Compute the index m = n * p_n. */
    idx_t m = n * p_n;

    /* Return the m‑th prime. */
    return nth_prime(m);
}
