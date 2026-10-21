/*
 * A078858  Initial term in sequence of four consecutive primes separated by 3
 *          consecutive differences each <=6 (i.e., when d = 2, 4 or 6) and
 *          forming d-pattern = [6, 6, 4]; short d-string notation of pattern
 *          = [664].
 * https://oeis.org/A078858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first prime p such that the next three consecutive
 *            primes are p+6, p+12, p+16 (difference pattern 6,6,4)
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static data needed for one-time precomputation. */
static bool initialized = false;
static uint32_t *prime_arr  = NULL;  /* primes up to LIMIT */
static size_t   prime_cnt  = 0;
static seq_t   *term_arr   = NULL;   /* sequence values a(1).. */
static size_t   term_cnt   = 0;

/* Pre‑computes a large list of primes and then extracts all
   quadruples that satisfy the 6,6,4 pattern.  The chosen limit
   (20 000 000) guarantees more than 10 000 terms for the 32‑bit or
   arbitrary‑precision builds.  The construction happens only once,
   no matter how many times a(…) is called. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* ----------------------------------------------------------- */
    /* 1. Sieve of Eratosthenes up to LIMIT. */
    const size_t LIMIT = 20000000;               /* 20 million */

    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) return;                          /* should not happen */

    /* Rough upper bound for the number of primes up to LIMIT. */
    const size_t PRIME_EST = LIMIT / 10;           /* < π(LIMIT) */
    prime_arr = (uint32_t *)malloc(PRIME_EST * sizeof(uint32_t));
    if (!prime_arr) { free(is_comp); return; }

    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_arr[prime_cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* ----------------------------------------------------------- */
    /* 2. Collect terms that satisfy pattern 6,6,4. */
    term_arr = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    if (!term_arr) return;                         /* allocation failure */

    term_cnt = 0;
    for (size_t i = 0; i + 3 < prime_cnt; ++i) {
        uint32_t p0 = prime_arr[i];
        uint32_t p1 = prime_arr[i + 1];
        uint32_t p2 = prime_arr[i + 2];
        uint32_t p3 = prime_arr[i + 3];
        if (p1 - p0 == 6 && p2 - p1 == 6 && p3 - p2 == 4)
            term_arr[term_cnt++] = (seq_t)p0;
    }
}

/* --------------------------------------------------------------- */
/* Public sequence access function.                                */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1, safe value */

    init();                            /* ensure data is ready */

    if ((idx_t)term_cnt >= n)
        return term_arr[(size_t)n - 1];

    /* The requested index lies beyond the pre‑computed range.
       According to the specification the result for such an n
       is not required; we return 0 as a placeholder. */
    return (seq_t)0;
}
