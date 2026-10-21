/*
 * A099407  Number of points of self-intersection of the path of a billiard
 *          ball traveling at a 45-degree angle on a prime(n) X prime(n+1)
 *          billiard table. Also equal to 1/2 the number of the lattice points
 *          lying within a prime(n) X prime(n+1) rectangle.
 * https://oeis.org/A099407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 1) * (prime(n+1) - 1) / 2
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n‑th prime, 1‑based.  */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime generator ------------------------------------------- */

static uint32_t *prime_list   = NULL;   /* 0‑based storage of primes   */
static size_t    prime_count  = 0;      /* number of primes stored    */
static size_t    prime_cap    = 0;      /* allocated capacity         */
static uint32_t  candidate     = 3;     /* next odd number to test    */

/* Ensure that prime_list contains at least primes[0 .. need]  (0‑based). */
static void
ensure_primes(size_t need)
{
    if (prime_count > need) return;          /* already enough primes */

    /* First time initialization. */
    if (!prime_list) {
        prime_cap   = 16;
        prime_list  = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[0] = 2;                   /* prime(1) = 2             */
        prime_count  = 1;
    }

    while (prime_count <= need) {
        /* Test whether candidate is prime. */
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p  = prime_list[i];
            uint64_t sq = (uint64_t)p * p;
            if (sq > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;                     /* double capacity */
                prime_list = (uint32_t *)realloc(prime_list,
                            prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = candidate;
        }

        candidate += 2;     /* skip even numbers */
    }
}

/* ---------- Sequence -------------------------------------------------- */

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe return */

    /* Need primes[n] -> prime(n+1)  (0‑based index), and prime[n-1] -> prime(n). */
    ensure_primes((size_t)n);      /* guarantees prime_list[0..n] exist */

    seq_t p1 = (seq_t)prime_list[n - 1];  /* prime(n)   */
    seq_t p2 = (seq_t)prime_list[n];      /* prime(n+1) */

    return ((p1 - 1) * (p2 - 1)) / 2;
}
