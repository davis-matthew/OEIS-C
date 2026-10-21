/*
 * A090183  a(n) = (10 - (prime(n+3) mod 10) mod 4) - (n mod 4) + 3.
 * https://oeis.org/A090183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ((10 - (prime(n+3) mod 10)) mod 4) - (n mod 4) + 3 where
 *            prime(k) is the k‑th prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Primes are generated incrementally; they are stored in a static array. */
static idx_t *prime_list   = NULL;   /* array of primes (1‑indexed in OEIS sense) */
static size_t prime_count  = 0;       /* number of primes stored */
static size_t prime_cap    = 0;       /* current allocated capacity */
static idx_t   next_candidate = 2;    /* next integer to test for primality */

/* Ensure that prime_list contains at least (up_to_index+1) primes
   (0‑based index).  If more are needed, generate them by simple trial
   division using the primes already known. */
static void ensure_primes(size_t up_to_index)
{
    while (prime_count <= up_to_index) {
        /* Grow the array if necessary.  */
        if (prime_cap == 0 || prime_count == prime_cap) {
            prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
            idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
            if (!tmp) return;          /* allocation failed – give up */
            prime_list = tmp;
        }

        /* Test next_candidate for primality.  */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if ((idx_t)p * (idx_t)p > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            prime_list[prime_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    /* We need the (n+3)-th prime (1‑based).  In zero‑based array terms this
       is prime_list[n+2].  */
    size_t index = (size_t)(n + 2);
    ensure_primes(index);

    idx_t p = prime_list[index];
    int pmod10 = (int)(p % 10);
    int t = ((10 - pmod10) % 4 + 4) % 4;          /* normalize to 0..3 */
    int res = t - (int)(n % 4) + 3;               /* result is 0..6 */

    return (seq_t)res;
}
