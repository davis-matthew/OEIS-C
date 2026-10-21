/*
 * A078773  a(n) is the largest prime less than or equal to phi(n), a(1) =
 *          a(2) = 0.
 * https://oeis.org/A078773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime <= phi(n), with a(1)=a(2)=0
 * Range:     u8 n <= 261, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses a small sieve of Eratosthenes to generate all
   primes up to a fixed limit.  For n <= 10 000 the value of phi(n) never
   exceeds 10 000, so the sieve limit of 20 000 is more than safe.  The
   phi calculation is performed by factoring n with the pre‑computed primes.
   The largest prime <= phi(n) is found by linear descent in the prime
   list (fast enough for the small limits used here). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool primes_initialized = false;
static uint32_t primes[20000];
static size_t prime_cnt = 0;

/* Simple sieve, all primes stored in the static array.  The limit is
   generous enough to cover phi(n) for n up to 10⁴. */
static void init_primes(void)
{
    if (primes_initialized) return;
    const uint32_t limit = 20000;
    bool is_prime[limit + 1];
    for (uint32_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p)
        if (is_prime[p])
            for (uint32_t multiple = p * p; multiple <= limit; multiple += p)
                is_prime[multiple] = false;

    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;

    primes_initialized = true;
}

/* Euler's totient function via simple factorisation. */
static seq_t phi_of(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* phi(1) = 1 */

    seq_t result = (seq_t)n;
    idx_t temp   = (idx_t)n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) result -= result / (seq_t)temp;

    return result;
}

/* Largest prime <= x (x fits in 32 bits for the problem constraints). */
static seq_t largest_prime_leq(seq_t x)
{
    /* Special case: no prime <= 1 */
    if (x < 2) return (seq_t)0;

    /* Binary search for the highest index where primes[idx] <= x */
    size_t lo = 0;
    size_t hi = prime_cnt;            /* exclusive */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((uint32_t)primes[mid] <= (uint32_t)x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    /* lo is the first index with prime > x, so lo-1 is last <= x */
    return (seq_t)primes[lo - 1];
}

seq_t a(idx_t n)
{
    init_primes();                      /* one-time initialization */

    if (n < 3) return (seq_t)0;         /* as per the OEIS definition */

    seq_t phi_val = phi_of(n);
    return largest_prime_leq(phi_val);
}
