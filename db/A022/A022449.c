/*
 * A022449  c(p(n)) where p(k) is k-th prime including p(1)=1 and c(k) is k-th
 *          composite number.
 * https://oeis.org/A022449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 5963, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = composite index of the nth "prime" (with p(1)=1), i.e.
   a(n) = A002808(A008578(n)) – OEIS A022449. */

#include <stdlib.h>
#include <stdint.h>

/* ---------- static storage for primes and composites ---------- */
static uint32_t *primes = NULL;     /* primes[0] = 2, primes[1] = 3, ... */
static size_t   primes_len = 0;     /* number of primes stored */

static seq_t   *composites = NULL;  /* composites[1] = 4, composites[2] = 6, ... */
static size_t   comps_len   = 0;    /* number of composites stored */
static size_t   sieve_limit = 0;    /* the numeric limit last used for the sieve */

/* ---------- helper: rebuild tables to satisfy at least
                 need_comp composites and need_prime primes ----------
*/
static void rebuild_tables(size_t need_comp, size_t need_prime)
{
    /* free previous arrays */
    free(primes);
    free(composites);
    primes = NULL;
    composites = NULL;
    primes_len = 0;
    comps_len = 0;
    sieve_limit = 0;

    /* Choose a starting limit */
    size_t limit = 100;
    if (limit < need_comp * 20U) limit = need_comp * 20U;
    if (limit < need_prime * 20U) limit = need_prime * 20U;
    if (limit < 100U) limit = 100U;

    /* Increase limit until we have enough primes and composites */
    while (1) {
        /* allocate prime flag array */
        char *is_prime = (char *)malloc(limit + 1);
        if (!is_prime) exit(1);          /* out of memory */

        /* beginning: mark all as prime */
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* sieve */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* count primes and composites */
        size_t cnt_prime = 0;
        size_t cnt_comp  = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) ++cnt_prime;
            else if (i >= 4) ++cnt_comp;
        }

        if (cnt_comp >= need_comp && cnt_prime >= need_prime) {
            /* Allocate and fill primes array */
            primes = (uint32_t *)malloc(cnt_prime * sizeof(uint32_t));
            if (!primes) exit(1);
            size_t pi = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (is_prime[i]) primes[pi++] = (uint32_t)i;
            primes_len = cnt_prime;

            /* Allocate and fill composites array (index 0 unused) */
            composites = (seq_t *)malloc((cnt_comp + 1) * sizeof(seq_t));
            if (!composites) exit(1);
            composites[0] = 0;  /* unused */
            size_t ci = 1;
            for (size_t i = 4; i <= limit; ++i)
                if (!is_prime[i]) composites[ci++] = (seq_t)i;
            comps_len = cnt_comp;

            sieve_limit = limit;
            free(is_prime);
            break;  /* accomplished */
        }

        /* not enough: increase limit and try again */
        free(is_prime);
        limit *= 2;
    }
}

/* ---------- main function for the sequence ----------
   seq_t a(idx_t n)
*/
seq_t a(idx_t n)
{
    /* initial build with some minimal tables */
    static int initialized = 0;
    if (!initialized) {
        rebuild_tables(1, 1); /* at least 1 composite and 1 prime */
        initialized = 1;
    }

    /* Determine the prime index to look up */
    if (n == 0) return 0;             /* offset is 1; undefined for n==0 */

    uint64_t k;                       /* composite index needed */
    if (n == 1) {
        k = 1;                         /* p(1)=1 => first composite */
    } else {
        /* need at least (n-1) primes for n>1 */
        if (primes_len < (size_t)(n - 2 + 1)) {   /* primes_len < n-1 */
            rebuild_tables(1, (size_t)(n - 1));
        }
        uint32_t p = primes[(size_t)(n - 2)];      /* p(n) */
        k = (uint64_t)p;
    }

    /* Ensure enough composites are stored */
    if (comps_len < k) {
        rebuild_tables((size_t)k, primes_len);
    }
    return composites[(size_t)k];
}
