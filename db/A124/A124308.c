/*
 * A124308  Primes indexed by 5-almost primes.
 * https://oeis.org/A124308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = prime(A014614(n)) A014614(n) is the nth 5‑almost prime
 *            (number with exactly five prime factors, counting multiplicity).
 *            a(n) is the prime whose index equals that 5‑almost prime
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes all 5‑almost primes up to a fixed
   limit and the primes up to the maximum 5‑almost prime index
   needed for the requested n.  It works for all n≥1 and can
   answer calls in any order.  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes.h"

static const idx_t SPF_LIMIT = 2000000ULL;     /* enough for all test n */
static const idx_t INIT_PRIME_LIMIT = 1000000ULL;

static idx_t *spf_arr      = NULL;   /* smallest prime factor for each i */
static uint8_t *omega_arr  = NULL;   /* Ω(i) for i ≤ SPF_LIMIT */
static idx_t *five_primes  = NULL;   /* indices i where Ω(i)=5  */
static size_t five_len     = 0;

static idx_t *prime_list      = NULL;   /* primes in ascending order */
static size_t prime_count     = 0;       /* number of primes stored      */
static idx_t current_prime_limit = 0;

/* --------------------------------------------------------------------- */
/* Build the spf table, fill omega, and collect all 5‑almost primes      */
static void init_five_primes(void)
{
    build_spf(SPF_LIMIT, &spf_arr);

    omega_arr = (uint8_t*)malloc((SPF_LIMIT + 1) * sizeof(uint8_t));
    if (!omega_arr) exit(EXIT_FAILURE);

    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (idx_t i = 2; i <= SPF_LIMIT; ++i) {
        idx_t sp = spf_arr[i];
        omega_arr[i] = omega_arr[i / sp] + 1;
    }

    size_t cap = 50000;  /* initial capacity for the 5‑almost prime list */
    five_primes = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!five_primes) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= SPF_LIMIT; ++i) {
        if (omega_arr[i] == 5) {
            if (five_len == cap) {
                cap <<= 1;
                five_primes = (idx_t*)realloc(five_primes, cap * sizeof(idx_t));
                if (!five_primes) exit(EXIT_FAILURE);
            }
            five_primes[five_len++] = i;
        }
    }

    free(spf_arr);
    free(omega_arr);
    spf_arr   = NULL;
    omega_arr = NULL;
}

/* --------------------------------------------------------------------- */
/* Ensure that we have at least 'need' primes stored in prime_list       */
static void extend_primes_to(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    idx_t limit = current_prime_limit == 0 ? INIT_PRIME_LIMIT
                                            : current_prime_limit * 2;
    while (prime_count < (size_t)need) {
        if (prime_list)
            free(prime_list);
        size_t cnt = 0;
        build_primes(limit, &prime_list, &cnt);
        prime_count = cnt;
        current_prime_limit = limit;
        if (prime_count >= (size_t)need)
            break;
        limit <<= 1;   /* double the limit for the next iteration */
    }
}

/* --------------------------------------------------------------------- */
/* One‑time global initialization                                       */
static void initialize(void)
{
    init_five_primes();

    /* Ensure the prime list is long enough to index the largest 5‑almost
       prime that we may need.  */
    idx_t max_5ap = (idx_t)0;
    if (five_len > 0)
        max_5ap = five_primes[five_len - 1];
    extend_primes_to(max_5ap);
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }

    if (n < 1 || (idx_t)n - 1 >= five_len)  /* undefined for n out of range */
        return 0;

    /* Step 1: nth 5‑almost prime value (the index of the desired prime). */
    idx_t m = five_primes[(size_t)n - 1];

    /* Step 2: ensure we have at least m primes. */
    if (prime_count < (size_t)m)
        extend_primes_to(m);

    /* Step 3: return the m‑th prime (1‑based index). */
    return (seq_t)prime_list[(size_t)m - 1];
}
