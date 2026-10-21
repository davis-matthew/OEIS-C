/*
 * A066909  (product of primes < n that do not divide n) (mod n).
 * https://oeis.org/A066909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (product of primes < n that do not divide n) modulo n
 * Range:     u8 n <= 294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*=== Prime cache =============================================================*/
static uint64_t sieve_limit   = 0;          /* maximum n already sieved            */
static uint8_t  *is_composite = NULL;       /* marks composite numbers             */
static uint32_t *prime_list   = NULL;       /* list of primes in ascending order   */
static size_t    prime_cnt    = 0;           /* number of primes stored             */

/* Rebuild prime table up to at least 'limit' (inclusive).  The table is
   extended only when a larger limit is requested. */
static void recompute_primes(uint64_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* Allocate and run a simple sieve up to 'limit' */
    uint8_t *new_flag = (uint8_t*)calloc((size_t)limit + 1, 1);   /* 0 = prime candidate */
    if (limit >= 1) {
        new_flag[0] = 1;
        if (limit >= 1) new_flag[1] = 1;
    }

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!new_flag[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                new_flag[j] = 1;
            }
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!new_flag[i]) ++cnt;
    }

    /* Allocate array for primes */
    uint32_t *new_primes = NULL;
    if (cnt > 0) {
        new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
        size_t idx = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (!new_flag[i]) new_primes[idx++] = (uint32_t)i;
        }
    }

    /* Release old storage and store the new one */
    free(is_composite);
    free(prime_list);
    is_composite = new_flag;
    prime_list   = new_primes;
    prime_cnt    = cnt;
    sieve_limit  = limit;
}

/*=== Sequence computation =====================================================*/
/* Computes a(n) as defined.  The function works for any build that
   provides seq_t (unsigned integer or arbitrary‑precision class) and
   idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* Guard against n == 0 (not used by the OEIS offset). */
    if (n == 0) return 0;

    /* Ensure all primes < n are available. */
    recompute_primes((uint64_t)n - 1);

    seq_t prod = 1;                     /* running product (mod n will be applied afterwards) */

    /* Multiply all primes < n that do not divide n. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p >= (uint64_t)n) break;  /* all remaining primes are >= n */
        if (n % (uint64_t)p != 0) {
            seq_t sp = (seq_t)p;
            prod = (prod * sp) % (seq_t)n;
        }
    }

    /* Modulo n for the empty product case (n = 1). */
    return prod % (seq_t)n;
}
