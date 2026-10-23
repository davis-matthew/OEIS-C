/*
 * A032601  a(n) = n-th prime number * n-th lucky number.
 * https://oeis.org/A032601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032601:  a(n) = n‑th prime * n‑th lucky number
   (A000040(n) * A000959(n))
   n is 1‑based; behaviour for n < 1 is undefined but harmless. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Prime caching                                               n <= 10000
   ------------------------------------------------------------------ */
static uint32_t *prime_cache = NULL;
static size_t   prime_len   = 0;

static void ensure_prime(idx_t n)
{
    if (n == (idx_t)0) return;
    if (prime_len >= (size_t)n) return;          /* already have enough */

    /* Rough upper bound for nth prime: n * (log n + log log n)  */
    size_t limit = (size_t)(n * 15 + 1000);        /* safe for n <= 10000 */
    if (limit < 20) limit = 20;

    while (1) {
        /* allocate boolean array for composite flags */
        uint8_t *is_comp = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        if (!is_comp) abort();

        /* simple sieve of Eratosthenes */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (!is_comp[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }

        /* count primes */
        size_t count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!is_comp[i]) ++count;

        if (count < n) {                      /* not enough, enlarge limit */
            free(is_comp);
            limit = limit * 2 + 1000;
            continue;
        }

        /* store primes in the cache */
        if (prime_cache) free(prime_cache);
        prime_cache = (uint32_t *)malloc(count * sizeof(uint32_t));
        if (!prime_cache) abort();

        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!is_comp[i]) prime_cache[idx++] = (uint32_t)i;

        prime_len = count;
        free(is_comp);
        break;
    }
}

/* ------------------------------------------------------------------ */
/*  Lucky number cache                                        n <= 10000
   ------------------------------------------------------------------ */
static uint32_t *lucky_cache = NULL;
static size_t   lucky_len   = 0;

static void ensure_lucky(idx_t n)
{
    if (n == (idx_t)0) return;
    if (lucky_len >= (size_t)n) return;          /* already enough */

    size_t estimate = (size_t)(n * 20 + 100);     /* rough over‑estimate */

    while (1) {
        size_t M = estimate * 2 + 100;           /* upper bound for candidate values */
        size_t odd_cnt = M / 2 + 1;              /* number of odds <= M */

        uint32_t *cur = (uint32_t *)malloc(odd_cnt * sizeof(uint32_t));
        uint32_t *tmp = (uint32_t *)malloc(odd_cnt * sizeof(uint32_t));
        if (!cur || !tmp) abort();

        /* fill cur with odd numbers starting from 1 */
        size_t pos = 0;
        for (size_t num = 1; num <= M; num += 2)
            cur[pos++] = (uint32_t)num;
        size_t sz = pos;

        /* lucky sieve */
        for (size_t i = 1; i < sz; ++i) {
            uint32_t step = cur[i];
            if ((size_t)step > sz) break;        /* no more removals possible */

            size_t new_sz = 0;
            for (size_t j = 0; j < sz; ++j) {
                if ((j + 1) % step == 0) continue;  /* remove every step‑th element */
                tmp[new_sz++] = cur[j];
            }
            /* swap arrays */
            uint32_t *swap = cur;
            cur = tmp;
            tmp = swap;
            sz = new_sz;
        }

        if (sz >= n) {                           /* enough lucky numbers obtained */
            if (lucky_cache) free(lucky_cache);
            lucky_cache = (uint32_t *)malloc(n * sizeof(uint32_t));
            if (!lucky_cache) abort();
            memcpy(lucky_cache, cur, n * sizeof(uint32_t));
            lucky_len = n;
            free(cur);
            free(tmp);
            break;
        } else {                                  /* not enough, enlarge search */
            free(cur);
            free(tmp);
            estimate *= 2;                       /* increase estimate */
        }
    }
}

/* ------------------------------------------------------------------ */
/*          Main sequence function                                   */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0, harmless */

    /* lazily compute primes and lucky numbers up to n */
    ensure_prime(n);
    ensure_lucky(n);

    seq_t prime_val = (seq_t)prime_cache[(size_t)(n - 1)];
    seq_t lucky_val = (seq_t)lucky_cache[(size_t)(n - 1)];
    return prime_val * lucky_val;
}
