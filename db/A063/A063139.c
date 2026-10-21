/*
 * A063139  Composite numbers which in base 3 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063139: Composite numbers whose base‑3 representation contains
   their largest proper factor as a substring.
   a(1) = 9, a(2) = 15, a(3) = 21, ... */

/* The algorithm:
   1. Pre‑compute the smallest prime factor (spf) up to a fixed limit using
      a linear sieve.  The largest proper divisor of m (m composite) is
      m / spf[m].
   2. For each composite m, check whether the base‑3 digits of the largest
      proper divisor appear contiguously inside the base‑3 digits of m.
      The check is done by converting both numbers to base‑3 digit arrays
      and doing a simple substring search.
   3. Store found terms in a static array so that repeated calls to a(n)
      are O(1).  The array is expanded lazily if the user queries a
      larger index.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Global configuration                                            */
/* ------------------------------------------------------------------ */

#define SIEVE_LIMIT 2000000U   /* sufficient for n ≤ 10000   */

/* Smallest prime factor table (spf[n] = smallest prime factor of n) */
static uint32_t *spf = NULL;
static uint32_t *primes = NULL;      /* primes list used during sieve      */
static uint32_t prime_cnt = 0;

/* Cache of sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Last integer examined while building the cache */
static idx_t last_checked = 0;

/* ------------------------------------------------------------------ */
/*  Utility functions                                               */
/* ------------------------------------------------------------------ */

static void init_sieve(void)
{
    if (spf != NULL) return;          /* already done */

    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);                /* allocation failed */

    /* Allocate space for the prime list.
       For SIEVE_LIMIT = 2 000 000 the number of primes is < 150 000. */
    primes = (uint32_t *)malloc(200000 * sizeof(uint32_t));
    if (!primes) exit(1);

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (uint32_t pj = 0; pj < prime_cnt; ++pj) {
            uint32_t p = primes[pj];
            if (p > spf[i] || i * (uint64_t)p > SIEVE_LIMIT) break;
            spf[i * p] = p;
        }
    }
}

/* Return true if n is composite */
static bool is_composite(uint32_t n)
{
    if (n < 2) return false;
    return spf[n] != n;
}

/* Convert an unsigned long long to base‑3 digits (most significant first).
   Returns the number of digits (len).  The caller supplies buffers of
   sufficient length. */
static int to_base3(uint64_t x, uint8_t *digits, int max_len)
{
    if (x == 0) {
        if (max_len < 1) return 0;
        digits[0] = 0;
        return 1;
    }
    uint8_t tmp[64];
    int len = 0;
    while (x > 0 && len < 64) {
        tmp[len++] = (uint8_t)(x % 3);
        x /= 3;
    }
    /* reverse to most‑significant‑first order */
    for (int i = 0; i < len; ++i)
        digits[i] = tmp[len - 1 - i];
    return len;
}

/* Check whether the base‑3 representation of big contains
   that of pat as a contiguous substring. */
static bool contains_base3_substring(uint64_t big, uint64_t pat)
{
    uint8_t dig_big[64], dig_pat[64];
    int len_big = to_base3(big, dig_big, 64);
    int len_pat = to_base3(pat, dig_pat, 64);

    if (len_pat == 0 || len_big < len_pat) return false;

    for (int i = 0; i <= len_big - len_pat; ++i) {
        bool ok = true;
        for (int j = 0; j < len_pat; ++j) {
            if (dig_big[i + j] != dig_pat[j]) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

/* Append a term to the cache, expanding capacity if needed. */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; value for 0 not defined */
    init_sieve();

    while (terms_len < n) {
        ++last_checked;

        if (last_checked <= 3) continue;           /* 2,3 are prime */
        if (is_composite((uint32_t)last_checked) == false) continue;

        uint32_t spf_val = spf[(uint32_t)last_checked];
        uint64_t largest = (uint64_t)last_checked / (uint64_t)spf_val;

        if (contains_base3_substring((uint64_t)last_checked, largest))
            push_term((seq_t)last_checked);
    }

    return terms[n - 1];
}
