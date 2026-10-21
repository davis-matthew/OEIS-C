/*
 * A237425  Denominators of A164555(n)/A027642(n) + A198631(n)/A006519(n+1).
 * https://oeis.org/A237425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 for n even, let k = n/2. Then a(n) = ∏_{p
 *            prime, p-1 | 2k} p (von Staudt–Clausen theorem – denominator of
 *            B_{2k}) for n odd (n≥3), let t = (n+1)/2. Then a(n) =
 *            2^{v₂(t)+1}, where v₂(t) is the exponent of the highest power of
 *            2 dividing t
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_PRIME_LIMIT   20001   /* covers n up to about 10000 */
#define MAX_PRIME_COUNT   2300    /* number of primes ≤ MAX_PRIME_LIMIT */

static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_count = 0;
static int primes_initialized = 0;

/* Simple Sieve of Eratosthenes to fill 'primes' array */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    static uint8_t sieve[MAX_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            primes[prime_count++] = i;
        }
    }
    primes_initialized = 1;
}

/* Return a( n ) according to the discussed formulas. */
seq_t a(idx_t n)
{
    init_primes();                      /* ensure primes are ready */

    /* Special first terms */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* --- even index (n = 2k) ---------------------------------- */
    if ((n & 1U) == 0) {                /* even n */
        idx_t k = n >> 1;               /* k = n/2 */
        idx_t M = k << 1;               /* M = 2k */
        seq_t result = (seq_t)1;

        /* multiply all primes p with (p-1) | M */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = primes[i];
            if (p > (uint32_t)(M + 1))
                break;
            uint32_t pm1 = p - 1;
            if (M % pm1 == 0)
                result *= (seq_t)p;
        }
        return result;
    }

    /* --- odd index (n = 2k+1, k ≥ 1) ----------------------------- */
    /* Special case n == 1 already handled above */
    idx_t t = (n + 1) >> 1;  /* t = (n+1)/2 = k+1, k ≥ 1 */

    /* Compute exponent of 2 dividing t (v₂(t)) */
    unsigned v2 = 0;
    idx_t tmp = t;
    while ((tmp & 1U) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* final exponent: 1 + v₂(t) */
    unsigned exp = v2 + 1;
    /* result = 2^exp */
    return (seq_t)1 << exp;
}
