/*
 * A120291  Numerator of determinant of n X n matrix with elements M[i,j] =
 *          (1+Prime[i])/Prime[i] if i=j and 1 otherwise.
 * https://oeis.org/A120291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = (1 + Σ_{k=1}^n prime(k)) / ∏_{k=1}^n prime(k) reduced to
 *            its numerator; equivalently a(n) = (1+Σ)/(gcd(1+Σ,∏)). The
 *            implementation generates the first n primes on demand, sums
 *            them, finds the gcd by testing each prime that divides (1+Σ),
 *            and returns the reduced numerator
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Store the list of primes found so far. */
static uint64_t *primes = NULL;
static size_t    prime_count   = 0;
static size_t    prime_capacity = 0;

/* Ensure that at least n primes are available in the primes[] array. */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    if (prime_capacity == 0) {
        prime_capacity = 16;
        primes = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        if (!primes)
            abort();
    }

    uint64_t candidate = (prime_count == 0) ? 2U : primes[prime_count - 1] + 1U;

    while (prime_count < (size_t)n) {
        bool isprime = true;
        for (size_t i = 0; i < prime_count &&
                          (uint64_t)primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_count >= prime_capacity) {
                prime_capacity *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    prime_capacity * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[prime_count++] = candidate;
        }
        ++candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;  /* defined for safety, but OIES offset starts at 1 */

    ensure_primes(n);

    seq_t sum = 0;
    for (size_t i = 0; i < (size_t)n; ++i)
        sum += (seq_t)primes[i];

    seq_t v = sum + 1;      /* 1 + Σ primes */

    /* Compute g = gcd(v, product of the n primes); we only need each
       prime factor once, so iterate primes and multiply g by p if p | v. */
    seq_t g = 1;
    uint64_t v_u64 = (uint64_t)v;   /* for comparison with 64‑bit primes */
    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t p = primes[i];
        if (p <= v_u64) {
            seq_t vp = (seq_t)p;          /* safe: vp <= v */
            if (v % vp == 0)
                g *= vp;
        }
    }

    return v / g;
}
