/*
 * A074671  Five-digit distinct-digit primes.
 * https://oeis.org/A074671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1641, u32 n <= 2529, u64 n <= 2529,
 *            u128 n <= 2529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074671: five–digit primes with all distinct digits.
   The function returns the n‑th term (1‑based). */

#include <stdbool.h>
#include <stdlib.h>

/* Build all five‑digit distinct‑digit primes once. */
static unsigned char  sieve[100000];          /* 0 = prime, 1 = composite */
static unsigned int    primes[3000];          /* temporary storage, 5‑digit primes */
static idx_t           primes_cnt;            /* number of stored primes */
static bool            built = false;

/* Construct the list of primes. */
static void build(void)
{
    /* Sieve of Eratosthenes up to 99 999. */
    for (int i = 0; i < 100000; ++i) sieve[i] = 0;
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i * i < 100000; ++i)
        if (!sieve[i])
            for (int j = i * i; j < 100000; j += i)
                sieve[j] = 1;

    /* Collect primes that have five distinct decimal digits. */
    primes_cnt = 0;
    for (int n = 10000; n < 100000; ++n) {
        if (!sieve[n]) {
            bool used[10] = {0};
            int x = n;
            bool distinct = true;
            while (x > 0) {
                int d = x % 10;
                if (used[d]) { distinct = false; break; }
                used[d] = true;
                x /= 10;
            }
            if (distinct) {
                primes[primes_cnt++] = (unsigned int)n;
            }
        }
    }
}

/* Retrieve the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (!built) { build(); built = true; }
    if (n == 0 || n > primes_cnt) return 0;   /* out of range */
    return (seq_t)primes[n - 1];
}
