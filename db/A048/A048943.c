/*
 * A048943  Product of divisors of n is a square.
 * https://oeis.org/A048943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer whose product of all divisors
 *            is a square
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------- */
/* The following code assumes seq_t and idx_t are already defined
   by the build system.  No redefinitions of those types are made. */
/* ------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static data ------------------------------------- */
static seq_t *seq = NULL;      /* array of found terms */
static size_t seq_len = 0;     /* number of terms stored  */
static size_t seq_cap = 0;     /* capacity of seq array   */

static uint32_t *spf = NULL;   /* smallest prime factor table */
static size_t spf_lim = 0;     /* current size of SPF table  */

static uint64_t next_check = 1;/* next integer to test       */
static bool init_done = false;/* initialization flag        */

/* ---------- helper functions -------------------------------- */
static void ensure_seq_cap(size_t new_cap)
{
    if (seq_cap < new_cap) {
        size_t old = seq_cap;
        seq_cap = new_cap;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        /* Nothing else needed; realloc preserves existing data */
    }
}

static void build_spf(size_t limit)
{
    spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;  spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_lim = limit + 1;
}

/* test whether the product of all divisors of n is a perfect square */
static bool product_is_square(uint64_t n)
{
    if (n == 1) return true;

    uint64_t x = n;
    uint64_t div_cnt = 1;          /* τ(n) – number of divisors */
    /* exponents of distinct primes – we assume ≤16 distinct primes for n ≤ 2e6 */
    int exps[16];
    int ecnt = 0;

    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        exps[ecnt++] = e;
        div_cnt *= (uint64_t)(e + 1);
    }

    for (int i = 0; i < ecnt; ++i) {
        /* condition: e * τ(n) must be a multiple of 4 */
        if (((uint64_t)exps[i] * div_cnt) & 3ULL)   /* check mod 4 quickly */
            return false;
    }
    return true;
}

/* generate terms until we have at least n of them */
static void extend_up_to(idx_t n)
{
    /* sieve limit – generous enough for n up to about 10000 */
    const size_t SIEVE_LIMIT = 2000000;

    if (!init_done) {
        build_spf(SIEVE_LIMIT);
        init_done = true;
    }

    while (seq_len < n) {
        if (next_check > SIEVE_LIMIT) {
            /* Too far – would need to rebuild a larger sieve. */
            /* For the intended ranges this should never happen. */
            break;
        }
        if (product_is_square(next_check)) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
}

/* ------------------------------------------------------------- */
/* Required public function                                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined; harmless return */
    extend_up_to(n);
    return seq[n - 1];
}
