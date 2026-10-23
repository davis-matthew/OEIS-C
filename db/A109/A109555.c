/*
 * A109555  prime(k) for those k where floor(2*(((prime(k + 1) -
 *          prime(k))*PrimePi(k)) mod (8*k)) / k) = m with m = 0.
 * https://oeis.org/A109555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(k) where k is the smallest index such that floor( 2
 *            * ( (prime(k+1)-prime(k)) * pi(k) mod (8*k) ) / k ) = 0, i.e.
 *            ((prime(k+1)-prime(k)) * pi(k)) mod (8*k) < k/2
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The following implementation precomputes all primes up to a
    fixed numeric bound with a simple sieve.  It then evaluates the
    condition for successive indices k until the required n‑th term
    is found.  The implementation is fully portable to C11 and
    C++17, uses only integer arithmetic, and respects the
    seq_t / idx_t types supplied by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* SIEVE_LIMIT must be large enough to provide at least
   1 + the maximum index of primes that may be queried.
   2,000,000 gives about 148,933 primes – more than enough
   for the typical size of this sequence. */
#define SIEVE_LIMIT 2000000

/* --------------------------------------------------------------------- */
/* Global data structures used by the sequence routine.  They are
   lazily initialised on the first call to a(). */
static uint8_t *g_is_prime = NULL;   /* 1 = prime, 0 = composite */
static seq_t *g_primes = NULL;       /* array of primes, 1‑based */
static idx_t  g_prime_count = 0;     /* number of primes stored */

/* pi_arr[i] = number of primes <= i, for 0 <= i <= SIEVE_LIMIT */
static idx_t *g_pi_arr = NULL;

/* --------------------------------------------------------------------- */
/* Build the sieve and auxiliary arrays only once.  This function is
   called from a() and is guaranteed to be re‑entered safely. */
static void build_sieve(void)
{
    if (g_is_prime != NULL) {
        return;   /* already built */
    }

    /* Allocate and initialise the prime flag array */
    g_is_prime = (uint8_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!g_is_prime) return;
    memset(g_is_prime, 1, SIEVE_LIMIT + 1);
    g_is_prime[0] = 0;
    g_is_prime[1] = 0;

    for (idx_t i = 2; i * i <= (idx_t)SIEVE_LIMIT; ++i) {
        if (g_is_prime[i]) {
            for (idx_t j = i * i; j <= (idx_t)SIEVE_LIMIT; j += i) {
                g_is_prime[j] = 0;
            }
        }
    }

    /* Count primes to allocate the prime list */
    idx_t estimate = SIEVE_LIMIT / 10 + 1000;   /* rough upper bound */
    g_primes = (seq_t*)malloc(estimate * sizeof(seq_t));
    if (!g_primes) return;

    /* Record primes into the list */
    g_prime_count = 0;
    for (idx_t v = 2; v <= (idx_t)SIEVE_LIMIT; ++v) {
        if (g_is_prime[v]) {
            if (g_prime_count >= estimate) {
                estimate <<= 1;
                seq_t *tmp = (seq_t*)realloc(g_primes, estimate * sizeof(seq_t));
                if (!tmp) return;
                g_primes = tmp;
            }
            g_primes[g_prime_count++] = (seq_t)v;
        }
    }

    /* Allocate and build the Pi array (prime counting function) */
    g_pi_arr = (idx_t*)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!g_pi_arr) return;
    g_pi_arr[0] = 0;
    for (idx_t i = 1; i <= (idx_t)SIEVE_LIMIT; ++i) {
        g_pi_arr[i] = g_pi_arr[i-1] + (g_is_prime[i] ? 1 : 0);
    }
}

/* --------------------------------------------------------------------- */
/* Return the n-th term of OEIS sequence A109555.                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based, undefined below 1 */

    build_sieve();

    idx_t found = 0;                /* how many qualifying primes seen */
    for (idx_t k = 1; ; ++k) {
        /* We need the (k)-th and (k+1)-th prime.
           Ensure we have at least k+1 primes in our list. */
        if (k + 1 >= g_prime_count) {
            /* If we run out of pre‑computed primes we could extend
               the sieve, but the chosen bound is sufficiently large
               for all required n.  Failing gracefully. */
            return 0;
        }

        seq_t pk  = g_primes[k-1];          /* prime(k)   */
        seq_t pk1 = g_primes[k];            /* prime(k+1) */
        seq_t diff = pk1 - pk;              /* difference */

        idx_t pi_k = g_pi_arr[k];           /* number of primes <= k */

        /* Compute X = (diff * pi_k) mod (8*k) */
        /* All intermediate values comfortably fit in seq_t. */
        seq_t prod = diff * (seq_t)pi_k;
        idx_t modbase = 8 * k;
        idx_t X = (idx_t)(prod % (seq_t)modbase);

        /* Condition: X < k/2, equivalent to floor(2*X/k)==0 */
        if ( (X * 2ULL) < k ) {
            ++found;
            if (found == n) {
                return pk;   /* the desired term */
            }
        }
    }
}
