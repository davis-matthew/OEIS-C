/*
 * A180107  Partial sums of terms in A180101.
 * https://oeis.org/A180107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} t(k) where t(0)=1, t(1)=0, t(2)=1 and for
 *            k≥3 t(k) equals the prime p such that p repeats for
 *            (next_prime(p) – prev_prime(p)) consecutive indices. In other
 *            words the prime p≥3 occupies a block of length (next(p) –
 *            prev(p)) and the block for p=2 has length 2
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include <stdint.h>      /* for fixed‑width integer types */


#define MAX_PRIME 200000

static uint32_t primes[MAX_PRIME + 1];
static size_t  prime_count = 0;

/* simple sieve of Eratosthenes */
static void
compute_primes(void)
{
    /* array indexed by prime value, value 0 means prime */
    static unsigned char is_composite[MAX_PRIME + 1];
    if (prime_count != 0) return;          /* already built */

    prime_count = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[prime_count++] = (uint32_t)i;
            if (i * (idx_t)i <= MAX_PRIME) {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i) {
                    is_composite[(size_t)j] = 1;
                }
            }
        }
    }
}

/*-----------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    compute_primes();   /* ensure primes are available */

    seq_t sum = 0;
    idx_t pos = 0;              /* number of terms already processed */

    /* handle the three initial terms explicitly */
    if (pos < n) { sum += (seq_t)1; pos++; }
    if (pos < n) { sum += (seq_t)0; pos++; }
    if (pos < n) { sum += (seq_t)1; pos++; }
    if (pos == n) return sum;

    /* start with the block for prime 2 */
    size_t  prime_idx    = 0;                 /* primes[prime_idx] == 2 */
    size_t  block_rem   = 2;                  /* remaining terms in current block */

    while (pos < n) {
        if (block_rem == 0) {                 /* move to the next prime block */
            ++prime_idx;
            /* compute block length for primes >= 3 */
            if (prime_idx + 1 < prime_count) {
                block_rem = (size_t)(primes[prime_idx + 1] -
                                      primes[prime_idx - 1]);
            } else {
                /* should not happen for n ≤ 10000, but guard anyway */
                block_rem = (size_t)(n - pos);
            }
        }

        sum += (seq_t)primes[prime_idx];
        --block_rem;
        ++pos;
    }

    return sum;
}
