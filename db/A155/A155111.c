/*
 * A155111  Odd numbers k such that composite(k) is odd.
 * https://oeis.org/A155111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A155111: odd numbers k such that composite(k) is odd.
    a(n) = n  if n is odd and the n-th composite number is odd; otherwise 0.
    The term a(1) is the first such k.  The function memoizes all
    qualifying indices up to a fixed sieve limit sufficient for the
    required range of n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* State variables */
static uint64_t *qual_idx   = NULL;   /* indices of qualifying k's      */
static size_t   qual_len   = 0;       /* number of stored indices       */
static size_t   qual_cap   = 0;       /* capacity of qual_idx array     */
static uint8_t *prime_flag = NULL;   /* sieve flag: 1 = prime, 0 = composite */
static bool     initialized = false;

/* Ensure qualification index array has enough capacity. */
static void ensure_capacity(size_t need)
{
    if (qual_cap < need) {
        size_t newcap = (qual_cap == 0) ? 1024 : qual_cap << 1;
        while (newcap < need) newcap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(qual_idx, newcap * sizeof(uint64_t));
        if (!tmp) abort();          /* out of memory */
        qual_idx = tmp;
        qual_cap = newcap;
    }
}

/* Build the sieve and collect qualifying k's. */
static void init_sequence(void)
{
    /* Sieve size: enough to produce many qualifying terms (≈441‑k terms). */
    const size_t SIEVE_LIMIT = 2000000U;  /* 2 million covers >10 000 terms */

    prime_flag = (uint8_t *)calloc(SIEVE_LIMIT + 1U, 1U);
    if (!prime_flag) abort();

    /* Mark primes: start with all marked as prime, then sieve. */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) prime_flag[i] = 1U;
    prime_flag[0] = prime_flag[1] = 0U;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                prime_flag[j] = 0U;
            }
        }
    }

    /* Scan for composites and collect qualifying indices. */
    size_t comp_idx = 0;  /* 1‑based index of composites */
    for (size_t num = 2; num <= SIEVE_LIMIT; ++num) {
        if (!prime_flag[num]) {         /* composite */
            ++comp_idx;                 /* fold into composite index */
            if ((comp_idx & 1U) && (num & 1U)) {
                ensure_capacity(qual_len + 1);
                qual_idx[qual_len++] = comp_idx; /* store k */
            }
        }
    }

    free(prime_flag);
    prime_flag = NULL;
    initialized = true;
}

/* Public API */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0) return 0;                     /* undefined for n=0 */

    size_t idx = (size_t)(n - 1U);             /* zero‑based array index */
    if (idx >= qual_len) return 0;             /* out of pre‑computed range */
    return (seq_t)qual_idx[idx];               /* cast to seq_t */
}
