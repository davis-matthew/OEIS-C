/*
 * A199516  Number of nX1 0..3 arrays with values 0..3 introduced in row major
 *          order, the number of instances of each value within one of each
 *          other, and every element equal to two or fewer horizontal or
 *          vertical neighbors.
 * https://oeis.org/A199516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let k = ⌊n/4⌋ and r = n mod 4. For n ≥ 4: a(n) = n! / (
 *            (k!)^(4−r) · ((k+1)!)^r · (4−r)! · r! ) For n < 4 (k = 0): a(n)
 *            = n! / r! (only the r non‑empty symbols of size 1 appear). The
 *            factorials of empty blocks (size 0) are omitted, and the
 *            factorial of the block multiplicities accounts for the un...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#ifndef MAX_PRIME_SCAN
#define MAX_PRIME_SCAN   20000U   /* sufficient for n up to ~10000 */
#endif

static idx_t primes[MAX_PRIME_SCAN + 1];
static size_t prime_cnt            = 0;
static idx_t primes_scanned_up_to  = 0;

static void init_primes_upto(idx_t limit)
{
    if (limit <= primes_scanned_up_to) return;
    if (limit > MAX_PRIME_SCAN) limit = MAX_PRIME_SCAN;
    static uint8_t sieve[MAX_PRIME_SCAN + 1] = {0};
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < MAX_PRIME_SCAN) primes[prime_cnt++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
    primes_scanned_up_to = limit;
}

static idx_t exp_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* counts A199516 for given n (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* not defined for n=0 */

    idx_t k = n / 4;
    idx_t r = n % 4;

    /* Determine block multiplicities.  For k==0 the empty blocks are ignored. */
    idx_t m0 = (k == 0) ? 0 : (4 - r);   /* blocks of size k   (empty blocks omitted) */
    idx_t m1 = r;                         /* blocks of size k+1 */

    /* Prepare primes up to n */
    init_primes_upto(n);

    seq_t result = 1;

    for (size_t pi = 0; pi < prime_cnt && primes[pi] <= n; ++pi) {
        idx_t p      = primes[pi];
        idx_t exp    = exp_in_factorial(n, p);          /* exponent from n!        */
        exp -= m0 * exp_in_factorial(k,   p);          /* subtract k! part       */
        exp -= m1 * exp_in_factorial(k + 1, p);        /* subtract (k+1)! part   */
        exp -= exp_in_factorial(m0,          p);        /* subtract m0! part      */
        exp -= exp_in_factorial(m1,          p);        /* subtract m1! part      */

        /* multiply the prime power p^exp into the result */
        for (idx_t e = 0; e < exp; ++e) {
            result *= (seq_t)p;
        }
    }

    return result;
}
