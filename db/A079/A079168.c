/*
 * A079168  Weighted quadratic roundness of n. If n=p_1^e_1...p^k_e^k, then
 *          a(n) = e_1 + (2^2 * e_2) + ... + (k^2 * e_k). Note that p_i<p_j,
 *          i<j, is assumed.
 * https://oeis.org/A079168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = weighted quadratic roundness of n a(n) = Σ_{i≥1} (i² ·
 *            e_i), where e_i is the exponent of the i‑th smallest prime
 *            dividing n. The sequence starts with a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

/* ------------------------------------------------------------------ */
/*  Prime generation utilities                                         */
/* ------------------------------------------------------------------ */

static idx_t *prime_list = NULL;
static size_t   prime_count = 0;
static idx_t   prime_limit = 0;   /* highest prime currently stored */

/* Ensure that `prime_list` contains all primes up to at least
   ⌊√n⌋ + 1.  The list is rebuilt only when a larger limit is needed. */
static void ensure_primes(idx_t n)
{
    if (n <= 1) return;

    /* compute integer √n */
    idx_t s = 0;
    while ((s + 1) * (s + 1) <= n) ++s;
    idx_t need = s + 1;                    /* primes up to this value */

    if (prime_limit >= need) return;       /* already sufficient */

    if (prime_list) free(prime_list);
    build_primes(need, &prime_list, &prime_count);
    prime_limit = need;
}

/* ------------------------------------------------------------------ */
/*  Sequence implementation                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1) = 0 */

    ensure_primes(n);

    seq_t sum  = 0;
    idx_t temp = n;
    idx_t idx  = 1;                        /* position among prime divisors */

    for (size_t i = 0; i < prime_count; ++i) {
        if (prime_list[i] > temp / prime_list[i]) break; /* beyond √temp */
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            sum += (seq_t)(idx * idx) * (seq_t)exp;
            ++idx;
        }
    }

    if (temp > 1) {                      /* remaining prime factor */
        sum += (seq_t)(idx * idx);       /* exponent assumed to be 1 */
    }

    return sum;
}
