/*
 * A100042  a(n) = prime(n)*2^prime(n).
 * https://oeis.org/A100042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = prime(n)*2^prime(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"

static bool is_composite[200001];
static idx_t primes_list[200000];
static idx_t primes_count = 0;

/* Builds a sieve of primes up to 200000 (enough for n≤10000). */
static void init_sieve(void)
{
    static bool initialized = false;
    if (initialized) return;
    const idx_t limit = 200000;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes_list[primes_count++] = i;
            if (i <= limit / i) {   /* avoid overflow on i*i */
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    init_sieve();
    if (n > primes_count) return (seq_t)0; /* out of range guard */
    idx_t p = primes_list[n - 1];
    seq_t pow = pow_seq((seq_t)2, (idx_t)p);
    return pow * (seq_t)p;
}
