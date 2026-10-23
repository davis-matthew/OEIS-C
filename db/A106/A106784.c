/*
 * A106784  Primes with digit sum = 56.
 * https://oeis.org/A106784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_sum_uint64, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime whose decimal digit sum is 56. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/isPrime64.h"
#include "../utils/digit_sum_uint64.h"

/* Buffer large enough to hold the first 20,000 such primes – this covers
   n up to 10,000 comfortably.  The array is statically allocated to avoid
   use of malloc/realloc which would not work for arbitrary‑precision
   seq_t types. */
static seq_t primes[20000];
static size_t prime_count = 0;        /* how many have been found */
static uint64_t next_candidate = 3;  /* start from the first odd prime */

/* Generate primes with digit sum 56 until at least n such primes are known. */
static void generate_until(idx_t n)
{
    while (prime_count < n)
    {
        if (isPrime64(next_candidate))
        {
            if (digit_sum_uint64(next_candidate) == 56)
            {
                primes[prime_count++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;          /* skip obvious even numbers */
    }
}

/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined at offset 0 – harmless */
    generate_until(n);
    return primes[(size_t)(n - 1)];
}
