/*
 * A211280  Numerator of prime(n+1) - prime(n)/2.
 * https://oeis.org/A211280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A211280
 *  Formula: a(1) = 2
 *           a(n) = 2*prime(n+1) - prime(n)   (n > 1)
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*  Prime generation helpers ------------------------------------------- */
/*  We use a one‑time sieve up to a fixed limit that is far beyond the
 *  prime needed for n <= 10000 (the 10001st prime is 104729).  The sieve
 *  generates all primes ≤ 200000, which yields 17984 primes – more than
 *  enough for our usage.  The primes are stored in a static array so
 *  subsequent calls to a() are O(1).  The array is initialized on the
 *  first call to a().
 */
#define SIEVE_LIMIT 200000U   /* adjust if a larger n range is needed */
#define MAX_PRIMES  20000U   /* number of primes that fit in the array */

static uint64_t *g_primes   = NULL;    /* array of primes (index 0 = prime(1)) */
static size_t   g_prime_cnt = 0;       /* number of primes generated          */
static bool     g_primes_ready = false;

/*  Internal routine to perform the sieve and populate g_primes.  It
 *  creates a temporary boolean array on the heap (freed immediately
 *  after use) and collects the primes into g_primes.
 */
static void init_primes(void)
{
    if (g_primes_ready) return;

    /*  Allocate temporary array for composite markers.  */
    bool *composite = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!composite) return;          /* allocation failure – leave empty */
    memset(composite, 0, (SIEVE_LIMIT + 1) * sizeof(bool));

    /*  Sieve of Eratosthenes.  */
    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                composite[j] = true;
        }
    }

    /*  Allocate the prime array.  */
    g_primes = (uint64_t*)malloc(MAX_PRIMES * sizeof(uint64_t));
    if (!g_primes) {
        free(composite);
        return;
    }

    /*  Collect primes into g_primes.  */
    g_prime_cnt = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT && g_prime_cnt < MAX_PRIMES; ++i) {
        if (!composite[i]) {
            g_primes[g_prime_cnt++] = i;
        }
    }

    free(composite);
    g_primes_ready = true;
}

/*  Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /*  Input is 1‑based; the function is required only for n >= 1.  */
    if (n == 0) return 0;            /* guard – undefined in OEIS, return 0 */

    /*  Ensure that the prime array is ready.  */
    if (!g_primes_ready) init_primes();

    /*  Special case n == 1.  */
    if (n == UINT64_C(1))
        return 2;

    /*  prime(n)   = g_primes[n-1]
     *  prime(n+1) = g_primes[n]          (array indices are zero based) */
    seq_t p_n  = (seq_t)g_primes[(size_t)n - 1];
    seq_t p_np = (seq_t)g_primes[(size_t)n];

    /*  a(n) = 2*prime(n+1) - prime(n)  */
    return (seq_t)((seq_t)2 * p_np - p_n);
}
