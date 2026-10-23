/*
 * A175130  Indices of Fibonacci numbers that are not cubefree.
 * https://oeis.org/A175130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 248, u32 n <= 248, u64 n <= 248,
 *            u128 n <= 248, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: indices of Fibonacci numbers that are not cubefree
 *
 * The function runs a sieve over all primes up to a fixed limit.
 * For each prime p it keeps Fibonacci(n) modulo p³.
 * Whenever the residue becomes 0 the corresponding index n is appended
 * to an increasing list of indices.  The function returns the nth term
 * of this list, using 1‑based indexing.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define PRIME_LIMIT 20000   /* enough for the first 10000 terms */

static int primes[PRIME_LIMIT];
static int prime_cnt = 0;
static bool primes_init = false;

static uint64_t *mod_arr = NULL;
static uint64_t *fprev_arr = NULL;
static uint64_t *fcurr_arr = NULL;

static idx_t *seq = NULL;      /* sorted list of indices */
static size_t seq_len = 0;     /* number of stored indices */
static size_t seq_cap = 0;     /* capacity of the seq array */
static idx_t cur_max = 1;      /* largest n already processed */

/* append a new term, expanding array if necessary */
static void push_term(idx_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(seq, new_cap * sizeof(idx_t));
        if (!tmp) return;   /* allocation failed – unlikely in this context */
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes_init) return;
    bool is_comp[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            primes[prime_cnt++] = i;

    mod_arr   = (uint64_t *)malloc(sizeof(uint64_t) * prime_cnt);
    fprev_arr = (uint64_t *)malloc(sizeof(uint64_t) * prime_cnt);
    fcurr_arr = (uint64_t *)malloc(sizeof(uint64_t) * prime_cnt);
    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        mod_arr[i]   = p * p * p;   /* p³, fits in 64‑bit for PRIME_LIMIT ≤ 20000 */
        fprev_arr[i] = 0;           /* F(0) */
        fcurr_arr[i] = 1;           /* F(1) */
    }
    primes_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but safe */

    init_primes();

    /* generate indices until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++cur_max;                /* next Fibonacci index to test */

        bool noncube = false;
        for (int i = 0; i < prime_cnt; ++i) {
            uint64_t mod = mod_arr[i];
            uint64_t next = fprev_arr[i] + fcurr_arr[i];
            /* since next < 2*mod, one subtraction is sufficient */
            if (next >= mod) next -= mod;
            fprev_arr[i] = fcurr_arr[i];
            fcurr_arr[i] = next;
            if (next == 0) noncube = true;
        }
        if (noncube) push_term(cur_max);
    }

    return (seq_t)seq[n - 1];
}
