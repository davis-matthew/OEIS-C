/*
 * A091178  Numbers k such that k-th prime is of the form 6*m+1.
 * https://oeis.org/A091178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index of the n‑th prime that satisfies prime ≡ 1 (mod
 *            6)
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*--- state --------------------------------------------------------------*/

static uint64_t *prime_list = NULL;     /* list of known primes (uint64_t) */
static size_t prime_cap   = 0;
static size_t prime_used  = 0;

static uint64_t *idx_list  = NULL;      /* list of indices k where prime(k) ≡ 1 mod 6 */
static size_t idx_cap      = 0;
static size_t idx_used     = 0;

static uint64_t candidate   = 2;        /* next number to test for primality */

/*--- helpers ------------------------------------------------------------*/

static void ensure_prime_cap(void)
{
    if (prime_used == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
}

static void ensure_idx_cap(void)
{
    if (idx_used == idx_cap) {
        idx_cap = idx_cap ? idx_cap * 2 : 16;
        idx_list = (uint64_t *)realloc(idx_list, idx_cap * sizeof(uint64_t));
    }
}

static void init_state(void)
{
    if (prime_list != NULL) return;      /* already initialized */

    /* initialise prime list with the first prime 2 */
    prime_cap  = 16;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    prime_list[0] = 2;
    prime_used  = 1;
    candidate   = 3;                     /* start testing odd numbers */

    idx_cap  = 16;
    idx_list = (uint64_t *)malloc(idx_cap * sizeof(uint64_t));
    idx_used = 0;
}

/* trial division primality test against known primes */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return (x == 2);
    size_t i;
    for (i = 0; i < prime_used; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* generate terms until we have at least n indices in idx_list */
static void generate_terms(idx_t n)
{
    while (idx_used < n) {
        /* find next prime */
        while (true) {
            uint64_t p = candidate;
            if (is_prime(p)) {
                ensure_prime_cap();
                prime_list[prime_used++] = p;
                /* prime index is prime_used (1‑based) */
                if (p % 6 == 1) {
                    ensure_idx_cap();
                    idx_list[idx_used++] = (uint64_t)prime_used;
                    /* next candidate is the following odd number */
                    candidate += 2;
                    break;
                }
                candidate += 2;
                continue;
            }
            candidate += 2;
        }
    }
}

/*--- exported function -------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not defined for n=0 */
    init_state();
    generate_terms(n);
    return (seq_t)idx_list[n - 1];
}
