/*
 * A088162  n-th prime rotated one binary place to the right less the n-th
 *          prime rotated one binary place to the left.
 * https://oeis.org/A088162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th prime rotated one binary place to the right) –
 *            (n-th prime rotated one binary place to the left)
 * Range:     u8 n <= 54, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum number of primes we may need for n up to about 10000
   (the 10000th prime is 104729).  20000 is safely sufficient. */
#define PRIME_ARRAY_SIZE 20000

static uint64_t primes[PRIME_ARRAY_SIZE];
static size_t prime_count = 0;

/* Ensure that primes[0..k-1] contains the first k primes. */
static void ensure_primes(idx_t k)
{
    if (prime_count >= (size_t)k) return;

    if (prime_count == 0) {
        primes[prime_count++] = 2;           /* first prime */
    }

    /* start candidate from the next odd number after the last known prime */
    uint64_t cand = primes[prime_count - 1] + 1;
    if ((cand & 1) == 0) ++cand;            /* guarantee odd */

    for (; prime_count < (size_t)k; cand += 2) {
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = primes[i];
            if (q * q > cand) break;        /* q > sqrt(cand) */
            if (cand % q == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            primes[prime_count++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */

    /* Compute nth prime if not yet present */
    ensure_primes(n);

    uint64_t p = primes[n - 1];           /* nth prime */

    /* Determine bit length of p (len >= 1). */
    uint64_t len = 0;
    uint64_t tmp = p;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Rotate right: move LSB to MSB. */
    uint64_t rotR = (p >> 1) | ((p & 1ULL) << (len - 1));

    /* Rotate left: move MSB to LSB. */
    uint64_t mask = ((uint64_t)1ULL << len) - 1;
    uint64_t rotL = ((p << 1) & mask) | (p >> (len - 1));

    return (seq_t)(rotR - rotL);
}
