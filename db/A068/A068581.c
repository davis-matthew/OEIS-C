/*
 * A068581  Let phi_m(x) = phi(phi(...(phi(x))...)) m times; sequence gives
 *          values of k such that phi_4(k) = tau(k).
 * https://oeis.org/A068581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068581: numbers k such that φ_4(k) = τ(k) (four iterated Euler totient equals divisor count).
   The function a(idx_t n) returns the nth such number (a(1)=1, a(2)=17,…).  Indices start at 1.
   The implementation searches integers k sequentially, caching results, and uses a simple
   prime sieve for factorisation.  It is valid in C11 and C++17 and works for any unsigned
   seq_t type provided by the build.  When the sequence ends (no further terms), a(n)
   returns 0 for n beyond the last found term. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------------------- */
/*  Prime sieve -------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
#define MAX_PRIMES 20000          /* large enough for primes up to ~2·10⁵   */

static uint64_t prime_list[MAX_PRIMES];
static size_t   prime_count = 0;

/* Generate all primes <= limit.  The list is stored in prime_list.       */
static void generate_primes(uint64_t limit)
{
    if (prime_count != 0) return;            /* already done */

    uint64_t cap = limit + 1;
    bool *is_prime = (bool *)calloc(cap, sizeof(bool));
    if (!is_prime) return;                  /* allocation failure – give up */

    for (uint64_t i = 2; i < cap; ++i) is_prime[i] = true;

    for (uint64_t i = 2; i * i < cap; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j < cap; j += i) is_prime[j] = false;
        }
    }

    prime_count = 0;
    for (uint64_t i = 2; i < cap && prime_count < MAX_PRIMES; ++i) {
        if (is_prime[i]) prime_list[prime_count++] = i;
    }

    free(is_prime);
}

/* ---------------------------------------------------------------------- */
/*  arithmetic helpers -------------------------------------------------- */
/* ---------------------------------------------------------------------- */

/* φ(n) for a single iteration.  Uses trial division over prime_list. */
static seq_t phi_one(seq_t n)
{
    seq_t result = n;
    seq_t m = n;
    for (size_t i = 0; i < prime_count && (seq_t)prime_list[i] * (seq_t)prime_list[i] <= m; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result = result / p * (p - 1);
        }
    }
    if (m > 1) result = result / m * (m - 1);
    return result;
}

/* τ(n) – divisor count.  Uses the same trial division. */
static seq_t tau_of(seq_t n)
{
    seq_t tau = 1;
    seq_t m = n;
    for (size_t i = 0; i < prime_count && (seq_t)prime_list[i] * (seq_t)prime_list[i] <= m; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) { m /= p; exp++; }
            tau *= (exp + 1);
        }
    }
    if (m > 1) tau *= 2;
    return tau;
}

/* φ applied m times.  For the sequence we always ask for m = 4. */
static seq_t phi_iterated(uint64_t k, unsigned m)
{
    seq_t x = (seq_t)k;
    for (unsigned i = 0; i < m; ++i) x = phi_one(x);
    return x;
}

/* ---------------------------------------------------------------------- */
/*  public routine ------------------------------------------------------ */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found values */
    static seq_t *values     = NULL;
    static size_t values_cap = 0;
    static size_t values_sz  = 0;
    static uint64_t next_k   = 1;   /* next integer to test */

    /* initialise prime sieve once – 200 000 is enough for all k tested below */
    generate_primes(200000);

    if (n == 0) return 0; /* undefined, but stay safe */

    /* grow cache as we find more terms */
    while (values_sz < (size_t)n) {

        uint64_t k = next_k++;
        seq_t phi4 = phi_iterated(k, 4);
        seq_t tau   = tau_of(k);

        if (phi4 == tau) {
            /* append to cache */
            if (values_sz == values_cap) {
                values_cap = values_cap ? values_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
                if (!tmp) return 0;                 /* allocation failure */
                values = tmp;
            }
            values[values_sz++] = (seq_t)k;
        }

        /* safety: stop after a generous upper bound (sequence is finite) */
        if (k > 10000000ULL && values_sz < (size_t)n) break;
    }

    if (n <= values_sz) return values[n-1];
    return 0;                     /* n is beyond the last known element */
}
