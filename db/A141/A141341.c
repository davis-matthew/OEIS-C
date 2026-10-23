/*
 * A141341  Totally Goldbach numbers: Positive integers k such that for all
 *          primes p < k-1 with p not dividing k, k-p is prime.
 * https://oeis.org/A141341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the n-th totally Goldbach number (OEIS A141341).
   It uses a simple incremental prime sieve and stores found terms for
   memoisation.  The code is valid in both C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime sieve infrastructure ----------------------------------- */

/* State of the global prime sieve */
static bool sieve_initialised = false;   /* has been built at least once?   */
static idx_t sieve_limit = 0;            /* largest number sieved so far   */
static uint8_t *isprime = NULL;          /* 0/1 flag array for primality   */
static uint32_t *prime_list = NULL;      /* array of primes                */
static size_t prime_cnt = 0;             /* number of primes stored        */
static size_t prime_cap = 0;             /* capacity of prime_list array   */

/* Ensure primes up to 'limit' (inclusive) are available. */
static void ensure_primes(idx_t limit)
{
    if (limit < 2) limit = 2;            /* smallest useful limit          */

    if (!sieve_initialised || limit > sieve_limit) {
        /* Allocate temporary array for new sieve result */
        uint8_t *tmp = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
        if (!tmp) exit(1);              /* allocation failure              */

        /* Initialise all entries to prime (=1), then mark 0 and 1 */
        for (idx_t i = 0; i <= limit; ++i) tmp[i] = 1;
        tmp[0] = tmp[1] = 0;

        /* Classic Eratosthenes */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (tmp[i]) {
                for (idx_t j = i * i; j <= limit; j += i) tmp[j] = 0;
            }
        }

        /* Build the prime list */
        size_t new_cap = limit / 10 + 100;      /* rough upper bound */
        uint32_t *pr = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!pr) exit(1);
        prime_list = pr;
        prime_cap = new_cap;

        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (tmp[i]) {
                if (cnt >= prime_cap) {          /* grow array if needed      */
                    prime_cap <<= 1;
                    pr = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                    if (!pr) exit(1);
                    prime_list = pr;
                }
                prime_list[cnt++] = (uint32_t)i;
            }
        }
        prime_cnt = cnt;

        /* Replace old sieve array */
        free(isprime);
        isprime = tmp;
        sieve_limit = limit;
        sieve_initialised = true;
    }
}

/* ------------------------------------------------------------------------ */

/* ---------- Sequence storage -------------------------------------------- */

/* State of the sequence terms */
static size_t term_cnt = 0;      /* number of terms stored           */
static seq_t *term_array = NULL;/* array of computed terms           */
static size_t term_cap = 0;      /* capacity of the array             */

static void grow_terms(size_t need)
{
    if (need <= term_cap) return;
    size_t new_cap = term_cap > 0 ? term_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(term_array, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    term_array = tmp;
    term_cap  = new_cap;
}

/* Current candidate k to test next */
static idx_t next_k = 1;

/* ---------- Check whether a given integer is Totally Goldbach ---------- */

static bool is_totally_goldbach(idx_t k)
{
    if (k < 2) return true;                /* 0 or 1 trivially satisfy       */

    idx_t limit = k - 2;                   /* primes < k-1                  */
    ensure_primes(limit);

    for (size_t idx = 0; idx < prime_cnt && prime_list[idx] <= limit; ++idx) {
        idx_t p = prime_list[idx];
        if (k % p != 0) {                  /* prime does not divide k       */
            if (!isprime[(size_t)(k - p)]) return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------------ */

/* ---------- The required sequence function ------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;           /* undefined for n <= 0          */

    /* Generate terms until we have at least n of them */
    while (term_cnt < n) {
        if (is_totally_goldbach(next_k)) {
            grow_terms(term_cnt + 1);
            term_array[term_cnt] = (seq_t)next_k;
            ++term_cnt;
        }
        ++next_k;
    }

    return term_array[n - 1];
}
