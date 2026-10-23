/*
 * A100091  Arithmetic mean of the first n primes and a minimal number greater
 *          than prime(n) such that an integer results.
 * https://oeis.org/A100091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (S(n)+M(n))/(n+1), * where S(n) is the sum of the first n
 *            primes, * and M(n) is the smallest integer > prime(n) * such
 *            that (S(n)+M(n)) is divisible by (n+1)
 * Range:     u8 n <= 13, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

static uint64_t *primes   = NULL; /* array of primes */
static seq_t   *cumsum    = NULL; /* cumulative sums of primes */
static size_t  count     = 0;      /* number of primes stored */
static size_t  capacity  = 0;      /* current capacity of the arrays */

static void ensure_primes(idx_t n)
{
    /* Grow the tables until at least n primes are stored. */
    while (count < n) {
        uint64_t candidate = (count == 0) ? 2 : primes[count - 1] + 1;
        /* Find the next prime. */
        while (!is_prime_uint64(candidate))
            ++candidate;

        if (count == capacity) {
            size_t newcap = capacity ? capacity * 2 : 64;
            uint64_t *newp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
            seq_t   *newc = (seq_t   *)realloc(cumsum, newcap * sizeof(seq_t));
            if (!newp || !newc) {
                /* Memory allocation failed – terminate the program. */
                exit(EXIT_FAILURE);
            }
            primes  = newp;
            cumsum  = newc;
            capacity = newcap;
        }

        primes[count] = candidate;
        cumsum[count] = (count == 0) ? (seq_t)candidate
                                    : cumsum[count - 1] + (seq_t)candidate;
        ++count;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence is defined for n ≥ 1 */

    ensure_primes(n);

    seq_t   sum = cumsum[n - 1];     /* sum of first n primes */
    uint64_t p_n = primes[n - 1];    /* n-th prime */

    /* Compute the required residue r = -(sum) mod (n+1). */
    idx_t mod_n1  = n + 1;
    idx_t sum_mod = (idx_t)(sum % (seq_t)mod_n1);          /* sum % (n+1) */
    idx_t r       = (mod_n1 - sum_mod) % mod_n1;          /* desired residue */

    /* Find the smallest M > p_n with M % (n+1) == r. */
    uint64_t M;
    if ((idx_t)r > p_n) {
        M = r;
    } else {
        idx_t q = (p_n - (idx_t)r) / mod_n1;              /* floor division */
        M = r + (q + 1) * mod_n1;
    }

    /* a(n) = (sum + M) / (n+1) */
    seq_t result = (sum + (seq_t)M) / (seq_t)mod_n1;
    return result;
}
