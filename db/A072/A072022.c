/*
 * A072022  Smallest x such that the number of nonprimes (i.e., 1 and
 *          composites) in the reduced residue set (RSS(x)) of x equals n, or
 *          0 if there are no such x.
 * https://oeis.org/A072022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 7659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072022 – Smallest x such that the number of non‑prime residues
 *  in the reduced residue set of x equals n.
 *  
 *  The number of non‑prime residues in RSS(x) is
 *      A048864(x) = φ(x) – π(x) + ω(x)
 *  where φ is Euler’s totient, π is the prime counting function
 *  and ω counts distinct prime factors of x.
 *  
 *  The routine precomputes φ, ω and π up to a fixed upper bound
 *  (2 000 000) with a linear sieve, then records, for each possible
 *  value of A048864(x), the smallest x that attains that value.
 *  If no such x exists the function returns 0.
 *  The same result works for any built‑in integer type that seq_t
 *  represents, including arbitrary‑precision.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- constants ----------------------------------------------------------*/
#define LIMIT     2000000UL    /* search upper bound */
#define MAX_N     200000UL     /* largest n we keep     */

/*--- static storage -----------------------------------------------------*/
static seq_t   *res      = NULL; /* result table 0..MAX_N      */
static uint32_t *phi     = NULL; /* φ(0..LIMIT)                 */
static uint8_t  *omega   = NULL; /* ω(0..LIMIT)                 */
static uint32_t *sp      = NULL; /* smallest prime factor       */
static uint32_t *prime_pref = NULL; /* π(0..LIMIT)             */
static uint32_t *primes  = NULL; /* list of primes              */
static size_t    primes_cnt = 0; /* number of primes found      */
static bool      initialized = false;

/*--- initialization -----------------------------------------------------*/
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate arrays */
    phi        = (uint32_t*)malloc((LIMIT+1) * sizeof(uint32_t));
    omega      = (uint8_t *)malloc((LIMIT+1) * sizeof(uint8_t));
    sp         = (uint32_t*)malloc((LIMIT+1) * sizeof(uint32_t));
    prime_pref = (uint32_t*)malloc((LIMIT+1) * sizeof(uint32_t));
    primes     = (uint32_t*)malloc((LIMIT+1) * sizeof(uint32_t));
    res        = (seq_t*)calloc(MAX_N+1, sizeof(seq_t));  /* 0 .. MAX_N */

    /* initialise small numbers */
    phi[0] = 0;  phi[1] = 1;
    omega[0] = 0; omega[1] = 0;
    sp[0] = 0;   sp[1] = 0;    /* 1 is not a prime */
    prime_pref[0] = 0;
    primes_cnt = 0;

    /* linear sieve for φ and ω */
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        if (sp[i] == 0)                     /* i is prime */
        {
            sp[i] = i;
            primes[primes_cnt++] = i;
            phi[i] = i - 1;
            omega[i] = 1;
        }
        for (size_t j = 0; j < primes_cnt &&
                           primes[j] <= sp[i] &&
                           (uint64_t)i * primes[j] <= LIMIT; ++j)
        {
            uint32_t p = primes[j];
            uint32_t v = i * p;
            sp[v] = p;
            if (i % p == 0)                 /* p divides i */
            {
                phi[v] = (uint32_t)((uint64_t)phi[i] * p);
                omega[v] = omega[i];
            }
            else
            {
                phi[v] = (uint32_t)((uint64_t)phi[i] * (p - 1));
                omega[v] = omega[i] + 1;
            }
        }
    }

    /* prefix array of prime counts π(x) – do NOT count 1 as a prime */
    for (uint32_t i = 1; i <= LIMIT; ++i)
        prime_pref[i] = prime_pref[i-1] + ((sp[i] == i && i > 1) ? 1U : 0U);

    /* compute minimal x for each value of A048864(x) */
    /* A048864(1) = 1  →  x = 1 is the first candidate       */
    res[1] = (seq_t)1;      /* handled explicitly */
    for (uint32_t x = 2; x <= LIMIT; ++x)
    {
        uint32_t nc = phi[x] - prime_pref[x] + omega[x];
        if (nc <= MAX_N && res[nc] == 0)
            res[nc] = (seq_t)x;
    }
}

/*--- public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* sequence is 1‑indexed; values <1 are undefined */
    if (n < 1) return 0;
    init();                /* ensure tables are ready     */
    return (n <= MAX_N) ? res[n] : 0;
}
