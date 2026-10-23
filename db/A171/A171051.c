/*
 * A171051  In the sequence of prime numbers, replace all the '9' digits with
 *          '2' and vice versa.
 * https://oeis.org/A171051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform( prime(n) ) where every decimal '9' is changed
 *            to '2' and every '2' is changed to '9' (all other digits stay
 *            unchanged)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

static seq_t *prime_cache      = NULL; /* cached primes, type seq_t */
static size_t prime_cache_cap  = 0;     /* allocated capacity of the cache   */
static size_t prime_cache_len  = 0;     /* number of primes actually stored  */

static seq_t transform_prime(seq_t p)
{
    seq_t res = 0;
    seq_t mul = 1;
    while (p != 0) {
        seq_t d = p % (seq_t)10;
        if (d == (seq_t)2) d = (seq_t)9;
        else if (d == (seq_t)9) d = (seq_t)2;
        res += d * mul;
        mul *= (seq_t)10;
        p /= (seq_t)10;
    }
    return res;
}

static void ensure_primes_upto(idx_t n)
{
    if (n <= prime_cache_len) return;   /* already have enough primes */

    /* estimate an upper bound for the nth prime */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        double lim = dn * (log(dn) + log(log(dn))) + 3.0; /* safety margin */
        limit = (idx_t)lim;
    }

    while (1) {
        unsigned char *is_composite = (unsigned char *)calloc((size_t)limit + 1,
                                                              sizeof(unsigned char));
        if (!is_composite) exit(1);          /* out‑of‑memory guard */
        size_t count = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                if (count >= prime_cache_cap) {
                    prime_cache_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
                    prime_cache = (seq_t *)realloc(prime_cache,
                                                   prime_cache_cap * sizeof(seq_t));
                }
                prime_cache[count++] = (seq_t)i;
                if ((seq_t)i * (seq_t)i <= (seq_t)limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_composite[j] = 1;
                }
            }
        }
        free(is_composite);
        prime_cache_len = count;
        if (prime_cache_len >= (size_t)n) break;   /* enough primes gathered */
        limit <<= 1;                              /* bound too small, retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; n==0 is undefined */
    ensure_primes_upto(n);
    seq_t p = prime_cache[(size_t)(n - 1)];
    return transform_prime(p);
}
