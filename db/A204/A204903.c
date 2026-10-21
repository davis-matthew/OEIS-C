/*
 * A204903  The odd prime q such that n divides p-q, where p>q is the least
 *          prime for which such a prime q exists.
 * https://oeis.org/A204903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For an index n>0, find the least prime p such that there exists
   an odd prime q<p with n|(p-q).  The function returns that odd prime q.
   It precomputes primes up to a fixed limit and scans them while keeping,
   for the current n, the largest odd prime seen for each residue modulo n.
   When a prime p has a residue already seen, the stored earlier prime is the
   required q. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------- prime sieve (up to a constant limit) --------- */
static const uint64_t PRIME_LIMIT = 2000000;      /* enough for n ≤ 10 000 */
static uint64_t *g_primes = NULL;                /* array of primes */
static size_t   g_prime_cnt = 0;                 /* number of primes */

/* Initialise the primes table once.  Works for both C and C++.    */
static void init_primes(void)
{
    if (g_primes != NULL)   /* already initialised */
        return;

    bool *is_prime = (bool *)malloc(PRIME_LIMIT + 1);
    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint64_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count primes first */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    g_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) g_primes[idx++] = i;
    g_prime_cnt = cnt;

    free(is_prime);
}

/* --------- sequence function --------- */
seq_t a(idx_t n)
{
    init_primes();                    /* ensure prime table ready */

    if (n == 0)                     /* out‑of‑range, return 0 */
        return (seq_t)0;

    /* Allocate an array for the largest odd prime seen for each residue */
    /* The array size is n; n is at most a few ten‑thousand, so this is safe */
    uint64_t *last_residue = (uint64_t *)calloc((size_t)n, sizeof(uint64_t));
    /* last_residue[r] == 0  → no odd prime seen with residue r yet */

    seq_t result = (seq_t)0;          /* placeholder */

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p = g_primes[i];

        /* Skip the even prime; q must be odd */
        if (p == 2)
            continue;

        uint64_t r = p % (uint64_t)n;           /* residue modulo n */

        uint64_t prev = last_residue[r];
        if (prev != 0) {                       /* a suitable q exists */
            result = (seq_t)prev;
            break;
        }

        /* Record this odd prime for future primes with same residue */
        last_residue[r] = p;
    }

    free(last_residue);

    /* The definition guarantees existence for n ≥ 1, but return 0
       defensively if something went wrong. */
    return result;
}
