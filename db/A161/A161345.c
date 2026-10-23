/*
 * A161345  Numbers k whose largest divisor <= sqrt(k) is 3.
 * https://oeis.org/A161345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*(prime(n-2)) for n>6, a(n) = 3*(n+2) for n<6, a(6) = 27
 *            (prime(1)=2, prime(2)=3, ...). This matches OEIS A161345
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Local storage for primes (indices are 1‑based).  */
static idx_t *primes = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static idx_t next_candidate = 2;

/* Grow the primes array until it contains at least k primes.  */
static void grow_primes_to(size_t k)
{
    while (prime_count < k) {
        /* Find the next prime after next_candidate.  */
        while (true) {
            bool is_prime = true;
            idx_t n = next_candidate;
            ++next_candidate;
            for (size_t i = 0; i < prime_count; ++i) {
                idx_t p = primes[i];
                if (p > n / p)  /* p*p > n */ break;
                if (n % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (prime_count == prime_cap) {
                    size_t new_cap = prime_cap == 0 ? 16 : prime_cap << 1;
                    primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                    prime_cap = new_cap;
                }
                primes[prime_count++] = n;
                break;  /* move to next candidate candidate */
            }
        }
    }
}

/* Return the k‑th prime (1‑based).  */
static idx_t get_prime(size_t k)
{
    grow_primes_to(k);
    return primes[k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe.   */
    if (n < 6) {
        /* 3*(n+2)  for n = 1..5 */
        return (seq_t)3 * (seq_t)(n + 2);
    }
    if (n == 6) {
        /* a(6) = 27 */
        return (seq_t)27;
    }
    /* n > 6: use third prime of index n-2 */
    idx_t p = get_prime((size_t)(n - 2));
    return (seq_t)3 * (seq_t)p;
}
