/*
 * A182519  a(n) = prime(n)^(n+1) - prime(n+1)^n.
 * https://oeis.org/A182519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^(n+1) - prime(n+1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

/* Cache of primes up to a fixed limit so that
   prime(n) and prime(n+1) can be retrieved quickly. */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static const uint64_t PRIME_LIMIT = 200000u;   /* enough for n up to 10000 */

static void init_prime_cache(void)
{
    if (prime_list == NULL)
    {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below the offset – safe guard */

    init_prime_cache();

    /* n is 1‑based, array is 0‑based */
    seq_t p1 = (seq_t)prime_list[n - 1];   /* prime(n)   */
    seq_t p2 = (seq_t)prime_list[n];      /* prime(n+1) */

    seq_t val1 = pow_uint((uint64_t)p1, n + 1);   /* prime(n)^(n+1) */
    seq_t val2 = pow_uint((uint64_t)p2, n);       /* prime(n+1)^n */

    return val1 - val2;                           /* a(n)        */
}
