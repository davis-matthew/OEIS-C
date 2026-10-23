/*
 * A020935  Greatest k such that (k-th prime) < (5 times n-th prime).
 * https://oeis.org/A020935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p_k with p_k < 5 · prime(n)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                         /* undefined for n < 1, safe fallback */

    /* 1‑based n‑th prime.  get_prime returns uint32_t. */
    uint32_t p_n = get_prime(n);

    /* 5·prime(n) – 1 safely in uint64_t (n ≤ 10000 ⇒ 5·prime(n) < 1 000 000). */
    uint64_t lim = (uint64_t)5 * (uint64_t)p_n;
    lim -= 1;                              /* we need primes strictly < 5·prime(n) */

    /* Return pi(lim) which counts primes ≤ lim, i.e. primes < 5·prime(n). */
    return compute_pi((idx_t)lim);
}
