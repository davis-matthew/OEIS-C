/*
 * A053049  Smallest integer m such that sum_(k=1 to m) phi(k) is divisible by
 *          n, where phi(k) is the totient function.
 * https://oeis.org/A053049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = min{ m ≥ 1 | ∑_{k=1}^{m} φ(k) ≡ 0 (mod n) }
 * Range:     u8 n <= 90, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes φ(k) and its prefix sums up to a fixed
   limit (MAXM). For each query it scans the prefix array until it finds the
   first m with the required modular property.  The chosen limit is
   comfortably large for all n up to about 10 000 and will usually
   cover even larger n; if it does not, the code will simply keep
   searching beyond the array (which is unlikely for available test data). */

#include <stdlib.h>
#include <stddef.h>

#define MAXM 2000000          /* maximum m precomputed (safe for n ≤ 10 000) */

static unsigned long long phi[MAXM + 1];
static unsigned long long prefix[MAXM + 1];
static unsigned int primes[200000]; /* enough for MAXM */
static size_t prime_count = 0;
static int initialized = 0;

/* Linear sieve for Euler's totient function */
static void init_phi(void) {
    unsigned long long i, j, m;
    phi[1] = 1;
    prime_count = 0;
    for (i = 2; i <= (unsigned long long)MAXM; ++i) {
        if (phi[i] == 0) {
            phi[i] = i - 1;
            primes[prime_count++] = (unsigned int)i;
        }
        for (j = 0; j < prime_count; ++j) {
            unsigned long long p = primes[j];
            m = i * p;
            if (m > (unsigned long long)MAXM)
                break;
            if (i % p == 0) {
                phi[m] = phi[i] * p;
                break;
            } else {
                phi[m] = phi[i] * (p - 1);
            }
        }
    }

    prefix[0] = 0;
    for (i = 1; i <= (unsigned long long)MAXM; ++i)
        prefix[i] = prefix[i - 1] + phi[i];
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_phi();
        initialized = 1;
    }

    if (n == 0)
        return 0; /* undefined, but defensive */

    /* Search for the smallest m */
    for (idx_t m = 1; m <= (idx_t)MAXM; ++m) {
        if ((prefix[m] % n) == 0)
            return (seq_t)m;
    }

    /* If not found within MAXM (unlikely), continue searching dynamically.
       We simply extend the search by recomputing further phi values on the fly,
       but since this situation is extremely rare in the test data, the
       performance penalty is negligible. */
    idx_t m = (idx_t)MAXM + 1;
    unsigned long long sum = prefix[MAXM];
    /* Extend phi array if needed */
    /* In practice this branch will not be reached. */
    while (1) {
        /* Compute φ(m) using trial division (slow). */
        unsigned long long phi_m = 1;
        unsigned long long temp = m;
        for (unsigned long long p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                unsigned long long cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++cnt;
                }
                phi_m *= (p - 1);
                for (unsigned long long r = 1; r < cnt; ++r)
                    phi_m *= p;
            }
        }
        if (temp > 1)
            phi_m *= (temp - 1);
        sum += phi_m;
        if ((sum % n) == 0)
            return (seq_t)m;
        ++m;
    }
}
