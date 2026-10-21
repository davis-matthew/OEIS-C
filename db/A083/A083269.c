/*
 * A083269  a(n) = pi(A052180(n)) = A000720(A052180(n)); subscript of last
 *          prime used in Eratosthenes sieve by which all composites between
 *          n-th and (n+1)th primes were excluded.
 * https://oeis.org/A083269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi( max_{p_n < k < p_{n+1}} lpf(k) ), where lpf(x) is the
 *            least prime divisor of a composite x (and lpf(1)=0)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define LIMIT 2000000                     /* upper bound for prime precomputation */

static uint32_t spf[LIMIT + 1];           /* smallest prime factor for each integer */
static uint32_t primes[200000];           /* list of primes in ascending order */
static size_t   prime_count = 0;          /* number of primes found so far */
static int      initialized = 0;          /* flag indicating whether sieve has run */

/* One‑time linear sieve to fill spf[] and primes[] */
static void init(void)
{
    if (initialized) return;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            primes[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i]) break;         /* keep spf[i] minimal */
            uint64_t v = (uint64_t)i * p;
            if (v > LIMIT) break;
            spf[v] = p;
        }
    }
    initialized = 1;
}

/* Return a(n) for OEIS sequence A083269 */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but safe */

    /* Safety check: ensure requested n is within computed primes. */
    if (n >= prime_count) return (seq_t)0;

    uint32_t p = primes[(size_t)(n - 1)];  /* the n-th prime (1‑based)     */
    uint32_t q = primes[(size_t)n];        /* the (n+1)-th prime          */
    uint32_t max_lpf = 0;

    /* Scan composites between p and q and track the largest least prime factor. */
    for (uint32_t x = p + 1; x < q; ++x) {
        uint32_t s = spf[x];
        if (s < x && s > max_lpf)          /* composite with larger lpf    */
            max_lpf = s;
    }

    if (max_lpf < 2) return (seq_t)0;     /* no composite found or lpf<2  */

    /* Binary search to find pi(max_lpf): the count of primes ≤ max_lpf. */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= max_lpf) lo = mid + 1;
        else                         hi = mid;
    }
    return (seq_t)lo;
}
