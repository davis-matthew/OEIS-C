/*
 * A053597  Let prime(i) = i-th prime (A000040), let d(i) =
 *          prime(i+1)-prime(i) (A001223); a(n) = number of distinct numbers
 *          among d(n), d(n+1), d(n+2), ... before first duplicate is
 *          encountered.
 * https://oeis.org/A053597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct prime gaps d(n)=p(n+1)-p(n) seen
 *            consecutively starting at n before the first duplicate occurs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- types supplied by the build ---------- */
typedef uint64_t idx_t;

/* ---------- static data used by a() ---------- */
static bool initialized = false;
static idx_t *primes = NULL;        /* array of primes, 0‑based: primes[0]=2 */
static size_t primes_cnt = 0;       /* number of primes stored */
static size_t primes_cap = 0;       /* current capacity of primes array */

/* ---------- helper: extend the primes array until it contains
   at least target_index (0‑based) ---------- */
static void extend_primes_to_index(idx_t target_index)
{
    if (target_index < primes_cnt)
        return;

    /* build primes incrementally by trial division */
    while (primes_cnt <= target_index) {
        idx_t candidate = primes_cnt == 0 ? 2 : primes[primes_cnt - 1] + 1;
        bool ok = false;
        while (!ok) {
            ok = true;
            for (idx_t i = 0; i < primes_cnt; ++i) {
                idx_t p = primes[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                ++candidate;
        }

        /* enlarge storage if needed */
        if (primes_cnt >= primes_cap) {
            size_t new_cap = primes_cap == 0 ? 32 : primes_cap * 2;
            idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
            if (!tmp) return;          /* allocation failure – should not happen */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = candidate;
    }
}

/* ---------- helper: initialise primes up to a fixed value ---------- */
static void init_primes(void)
{
    if (initialized)
        return;
    initialized = true;

    /* initial sieve up to a moderate limit (enough for n <= 10 000)          */
    const idx_t SIEVE_LIMIT = 300000U;               /* value (not count)    */
    bool *sieve = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve)
        return;                                      /* allocation failure */

    primes_cap = 20000;
    primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) {
        free(sieve);
        return;
    }

    primes_cnt = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) {
                    free(sieve);
                    return;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = i;
            if (i * i <= SIEVE_LIMIT)
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve[j] = true;
        }
    }
    free(sieve);
}

/* ---------- the sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1; values below are undefined */
        return 0;

    init_primes();

    /* We may need a few primes beyond n to detect the first repeat.      */
    /* Extend to at least (n-1)+200 to be safe.                           */
    idx_t needed_index = (n - 1) + 200;
    if (needed_index >= primes_cnt)
        extend_primes_to_index(needed_index);

    /*  Since the number of distinct gaps before a repeat is small,
        we use a tiny fixed array to keep the seen gaps.                 */
    int seen[64];
    int seen_cnt = 0;

    for (idx_t i = n - 1; ; ++i) {
        if (i + 1 >= primes_cnt) {
            /* not enough primes yet – extend a little further            */
            extend_primes_to_index(i + 200);
            if (i + 1 >= primes_cnt)
                break;   /* should not happen for typical inputs          */
        }

        int gap = (int)(primes[i + 1] - primes[i]);

        /* linear search for existing gap */
        bool found = false;
        for (int j = 0; j < seen_cnt; ++j) {
            if (seen[j] == gap) {
                found = true;
                break;
            }
        }
        if (found)
            break;          /* first duplicate encountered */

        if (seen_cnt < (int)sizeof(seen) / (int)sizeof(seen[0]))
            seen[seen_cnt++] = gap;
        else
            break;          /* should not happen – protect against overflow   */
    }

    return (seq_t)seen_cnt;
}
