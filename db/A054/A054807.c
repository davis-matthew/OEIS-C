/*
 * A054807  Fourth term of strong prime quartets: prime(m+1)-prime(m) >
 *          prime(m+2)-prime(m+1) > prime(m+3)-prime(m+2).
 * https://oeis.org/A054807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(A054806(n)) Where A054806(n) is the first prime
 *            of a strong prime quartet, and a(n) is the fourth (last) prime
 *            of that quartet. The sequence consists of the fourth prime of
 *            every 4‑tuple of consecutive primes with strictly decreasing
 *            gaps: p1<p2<p3<p4 and (p2‑p1) > (p3‑p2) > (p4‑p3)
 * Range:     u8 n <= 4, u16 n <= 900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – allowed in C and C++ */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/* Global data – keep separate from the function to allow
   static initialization on first call.                     */
static seq_t *term_list  = NULL;   /* array of a(n) terms   */
static size_t term_count   = 0;   /* number of terms stored */
static size_t term_cap     = 0;   /* current capacity       */

static uint64_t *prime_list = NULL; /* array of generated primes   */
static size_t prime_count   = 0;    /* number of primes stored    */
static size_t prime_cap     = 0;    /* current capacity           */

static uint64_t last_prime = 0;     /* largest prime generated so far */

/* Forward declarations */
static void init_sequence(void);

/* ------------------------------------------------------------------ */
/*   helper: extend the prime list by generating the next prime(s) of
   the sieve after the current limit.  This is used when the requested
   n is larger than the terms found in the initial sieve.             */
static void generate_next_prime(void)
{
    /* Start scanning from the next odd number after the last prime.
       We avoid checking even numbers except 2.                     */
    uint64_t candidate = last_prime + 2;
    /* trivial optimisation: if last_prime==2, start from 3 */
    if (candidate <= 2) candidate = 3;

    while (1) {
        uint64_t sqrt_c = (uint64_t)sqrt((double)candidate) + 1;
        int is_prime = 1;
        for (size_t i = 0; i < prime_count && prime_list[i] <= sqrt_c; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) break;          /* found a new prime */
        candidate += 2;               /* skip even numbers */
    }

    /* Append the new prime to the array */
    if (prime_count >= prime_cap) {
        prime_cap = (prime_cap == 0) ? 32 : prime_cap * 2;
        prime_list = (uint64_t *)realloc(prime_list,
                                         prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_count++] = candidate;
    last_prime = candidate;

    /* Check if this prime completes a qualifying quartet.
       The quartet consists of primes at indices: i-3,i-2,i-1,i.
       We only need to test the newest possible window.          */
    if (prime_count >= 4) {
        size_t i = prime_count - 1; /* index of the newest prime */
        uint64_t g1 = prime_list[i-1] - prime_list[i-2];
        uint64_t g2 = prime_list[i-2] - prime_list[i-3];
        uint64_t g3 = prime_list[i]   - prime_list[i-1];
        if (g1 > g2 && g2 > g3) {
            /* store the fourth prime of the quartet */
            if (term_count >= term_cap) {
                term_cap = (term_cap == 0) ? 32 : term_cap * 2;
                term_list = (seq_t *)realloc(term_list,
                                             term_cap * sizeof(seq_t));
            }
            term_list[term_count++] = (seq_t)prime_list[i];
        }
    }
}

/* ------------------------------------------------------------------ */
/*   initialise the static data structures by sieving primes up to a
   moderate limit and collecting the first few terms of the sequence. */
static void init_sequence(void)
{
    static int initialised = 0;
    if (initialised) return;
    initialised = 1;

    /* --- 1. sieve primes up to a reasonable limit (10 000 000) --- */
    const unsigned int LIMIT = 10000000U;      /* 10 million   */
    unsigned char *is_prime = (unsigned char *)malloc(LIMIT + 1);
    if (!is_prime) return;                    /* fallback on failure */

    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (unsigned int i = 2; (unsigned long long)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (unsigned long long j = (unsigned long long)i * i;
                 j <= LIMIT; j += i) {
                is_prime[(size_t)j] = 0;
            }
        }
    }

    /* allocate initial capacity for primes and terms arrays */
    prime_cap = 700000;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    term_cap   = 200000;
    term_list  = (seq_t *)malloc(term_cap * sizeof(seq_t));

    /* --- 2. collect primes from the sieve --- */
    for (unsigned int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count >= prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = (uint64_t)i;
        }
    }
    last_prime = prime_list[prime_count - 1];
    free(is_prime);                         /* sieve buffer no longer needed */

    /* --- 3. identify all qualifying quartets within the sieved range
         and store their fourth primes in term_list --- */
    for (size_t i = 0; i + 3 < prime_count; ++i) {
        uint64_t g1 = prime_list[i+1] - prime_list[i];
        uint64_t g2 = prime_list[i+2] - prime_list[i+1];
        uint64_t g3 = prime_list[i+3] - prime_list[i+2];
        if (g1 > g2 && g2 > g3) {
            if (term_count >= term_cap) {
                term_cap *= 2;
                term_list = (seq_t *)realloc(term_list,
                                             term_cap * sizeof(seq_t));
            }
            term_list[term_count++] = (seq_t)prime_list[i+3];
        }
    }
}

/* ------------------------------------------------------------------ */
/*   public function required by the interface                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but keep behaviour sane */

    init_sequence();               /* ensure the static arrays are ready */

    /* generate additional terms until we have at least n of them */
    while ((size_t)n > term_count) {
        generate_next_prime();
    }

    return term_list[(size_t)n - 1];
}
