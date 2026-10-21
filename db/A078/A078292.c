/*
 * A078292  Least nontrivial multiple of the n-th prime beginning with 8.
 * https://oeis.org/A078292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: for the n‑th prime p compute the smallest
 * non‑trivial multiple of p that starts with digit 8.
 * The multiple is k*p with k ≥ 2.
 * We iterate over powers of 10: find the smallest k such that
 * 8·10^m ≤ k·p ≤ 9·10^m – 1.  Once found we return k·p. */

#include <stddef.h>
#include <stdint.h>

#define PRIME_LIMIT 200000      /* sufficient to contain at least 10000 primes   */
#define MAX_PRIMES 20000        /* number of primes that can be stored          */

static uint32_t prime_list[MAX_PRIMES];
static size_t   prime_count  = 0;
static int      primes_built = 0;

/* Build a list of the first primes up to PRIME_LIMIT (once). */
static void build_prime_list(void)
{
    if (primes_built)
        return;

    static uint8_t sieve[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    prime_count = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            if (prime_count < MAX_PRIMES)
                prime_list[prime_count++] = (uint32_t)i;
        }
    }
    primes_built = 1;
}

/* Return the nth term of A078292 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but guard against 0 */

    build_prime_list();

    if ((idx_t)n > (idx_t)prime_count)
        return (seq_t)0;          /* should not happen for n ≤ 10000 */

    /* n‑th prime (1‑based) */
    seq_t p = (seq_t)prime_list[(size_t)(n - 1)];

    /* Search for the least multiple starting with 8. */
    seq_t pow10 = (seq_t)1;       /* 10^m */

    for (;;) {
        /* lower bound for k */
        seq_t prod8  = (seq_t)8 * pow10;
        seq_t low    = (prod8 + p - 1) / p;   /* ceil(prod8 / p)  */
        seq_t k      = low;
        if (k < (seq_t)2)
            k = (seq_t)2;                       /* non‑trivial multiple */

        /* upper bound for k */
        seq_t prod9 = (seq_t)9 * pow10;
        seq_t high  = (prod9 - 1) / p;           /* floor((9*pow10-1)/p) */

        if (k <= high)               /* found a suitable multiple */
            return k * p;

        /* Move to the next magnitude of 10. */
        pow10 = pow10 * (seq_t)10;
    }
}
