/*
 * A175967  Lexicographically earliest sequence with first differences as
 *          increasing sequence of nonprimes A018252.
 * https://oeis.org/A175967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + A018252(n-1), a(1) = 1
 * Range:     u8 n <= 18, u16 n <= 321, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = 1 + sum_{k=1}^{n-1} A018252(k) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- static storage for memoisation ---------------------------------- */

static seq_t *sums          = NULL;   /* sums[i] = sum of first i nonprimes, i >= 0 */
static size_t sums_cap      = 0;
static size_t sums_len      = 0;      /* number of valid entries in sums */

static uint64_t *primes     = NULL;   /* list of primes found so far */
static size_t primes_cap    = 0;
static size_t primes_cnt    = 0;

static uint64_t candidate   = 2;      /* integer to test next */

/* --- helper to grow the sums array ---------------------------------- */

static void ensure_sums_cap(size_t needed)
{
    if (sums_cap < needed) {
        size_t new_cap = sums_cap ? sums_cap * 2 : needed;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(sums, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        sums = tmp;
        sums_cap = new_cap;
    }
}

/* --- helper to grow the primes array --------------------------------- */

static void ensure_primes_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : needed;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* --- initialise the static arrays on first use ---------------------- */

static void init_caches(void)
{
    if (!sums) {
        sums_cap = 4;
        sums = (seq_t*)malloc(sums_cap * sizeof(seq_t));
        if (!sums) abort();
        sums[0] = 0;      /* sum of zero nonprimes */
        sums[1] = 1;      /* first nonprime is 1 */
        sums_len = 2;
    }
    if (!primes) {
        primes_cap = 4;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
        primes_cnt = 0;
    }
}

/* --- add the next nonprime number (>1) to the caches ---------------- */

static void add_next_nonprime(void)
{
    for (;;) {
        bool is_prime = true;
        uint64_t cand = candidate;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;          /* p*p > cand */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {                     /* prime: store and move on */
            ensure_primes_cap(primes_cnt + 1);
            primes[primes_cnt++] = cand;
        } else {                             /* composite: store as nonprime */
            /* grow sums to hold one more entry */
            ensure_sums_cap(sums_len + 1);
            sums[sums_len] = sums[sums_len - 1] + (seq_t)cand;
            sums_len++;
        }

        candidate++;                        /* next integer to test */
        if (!is_prime) return;              /* nonprime found, exit */
    }
}

/* --- the required function ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but not used */

    init_caches();

    /* guarantee that sums[n-1] is known (need n-1 nonprimes) */
    while (sums_len <= (size_t)(n - 1))
        add_next_nonprime();

    return sums[n - 1] + (seq_t)1;
}
