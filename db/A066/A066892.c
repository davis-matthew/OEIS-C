/*
 * A066892  Card (k : prime(k+n)-prime(k)=0 mod(k)).
 * https://oeis.org/A066892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k≥1 : prime(k+n)-prime(k) ≡ 0 (mod k) }
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate enough primes by a sieve and count the k satisfying the
   divisibility condition.  The sieve is done once and reused for subsequent calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Adjust these limits if more primes are required. */
#define MAX_PRIME_COUNT 600000          /* number of primes that will be stored          */
#define PRIME_SIEVE_LIMIT 8000000       /* maximum value to sieve (>= 8·10^6)             */

static int primes[MAX_PRIME_COUNT];
static size_t primes_cnt = 0;
static unsigned char sieve_arr[PRIME_SIEVE_LIMIT+1];
static bool primes_ready = false;

/* Sieve of Eratosthenes to generate primes up to PRIME_SIEVE_LIMIT
   and store the first MAX_PRIME_COUNT primes. */
static void init_primes(void)
{
    size_t i, j;
    memset(sieve_arr, 1, sizeof(sieve_arr));
    sieve_arr[0] = sieve_arr[1] = 0;
    for (i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve_arr[i]) {
            for (j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve_arr[j] = 0;
        }
    }
    primes_cnt = 0;
    for (i = 2; i <= PRIME_SIEVE_LIMIT && primes_cnt < MAX_PRIME_COUNT; ++i)
        if (sieve_arr[i])
            primes[primes_cnt++] = (int)i;
    /* In case the limit was insufficient, it would be necessary to grow the sieve,
       but for the intended ranges it should not happen. */
    primes_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!primes_ready)
        init_primes();

    /* Limit for k: we need p_{k+n} so k must satisfy k+n ≤ primes_cnt */
    size_t k_max = primes_cnt - (size_t)n;
    if (k_max == 0) return (seq_t)0; /* safety */

    seq_t count = 0;
    /* iterate over k from 1 to k_max (inclusive) */
    for (size_t k = 1; k <= k_max; ++k) {
        size_t p1 = (size_t)primes[k-1];
        size_t p2 = (size_t)primes[(k+n)-1];
        size_t diff = p2 - p1;
        if ((diff % k) == 0)
            count++;
    }
    return count;
}
