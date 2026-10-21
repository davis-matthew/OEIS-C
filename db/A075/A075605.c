/*
 * A075605  Smallest k not a palindrome and not divisible by 10 such that k
 *          and R(k) (A004086) both are divisible by the n-th prime.
 * https://oeis.org/A075605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   get_prime, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A075605
   For a given n ≥ 1 let p = the n‑th prime.
   a(n) is the smallest integer k that satisfies:
      – k is not a palindrome (k ≠ rev(k))
      – k is not divisible by 10
      – both k and rev(k) are divisible by p
   The function memoises already computed terms for efficiency. */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/reverse_decimal.h"

/* Compute the n‑th term. n is 1‑based. */
static seq_t compute_term(idx_t n)
{
    uint32_t pn = get_prime(n);      /* n‑th prime (small enough for uint32_t) */
    seq_t   p  = (seq_t)pn;          /* promote to seq_t */

    /* Search multiples of p */
    for (idx_t m = 1; ; ++m) {
        seq_t k = p * (seq_t)m;      /* candidate k */

        if ((k % 10) == 0)          /* skip multiples of 10 */
            continue;

        seq_t rev = reverse_decimal(k);
        if (rev == k)               /* skip palindromes */
            continue;

        if (rev % p == 0)           /* both k and rev divisible by p */
            return k;
    }
}

/* Memoised cache of computed terms */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)                     /* safety for out‑of‑range input */
        return 0;

    if (n > cache_size) {
        cache = (seq_t *)realloc(cache,
            sizeof(seq_t) * (n + 1));   /* cast ensures C++ compatibility */
        for (idx_t i = cache_size + 1; i <= n; ++i)
            cache[i] = compute_term(i);
        cache_size = n;
    }
    return cache[n];
}
