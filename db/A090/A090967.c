/*
 * A090967  Given the sequence of the sums of the divisors of the semiprimes,
 *          this is the subsequence where each sum is an even number.
 * https://oeis.org/A090967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the two prime factors of the n-th semiprime
 *            (counting multiplicity) whose sum is even. This is the
 *            subsequence of the sums of the prime factors of semiprimes,
 *            keeping only the even sums. The sequence starts at a(1)=4
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static const idx_t MAX_NUM = 2000000;          /* sieve upper bound */

/* Least prime factor array, built once */
static uint32_t *spf = NULL;

/* Cache of computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;        /* number of terms stored */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t last_checked = 4;      /* next number to inspect for semiprime */

/* Build smallest‑prime‑factor sieve up to MAX_NUM */
static void init_spf(void)
{
    if (spf)
        return;
    spf = (uint32_t *)malloc(sizeof(uint32_t) * (MAX_NUM + 1));
    if (!spf)
        exit(1);
    for (idx_t i = 2; i <= MAX_NUM; ++i)
        spf[i] = (uint32_t)i;
    for (idx_t i = 2; (idx_t)(i * i) <= MAX_NUM; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= MAX_NUM; j += i)
                if (spf[j] == j)
                    spf[j] = (uint32_t)i;
}

/* Ensure that cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;
    init_spf();

    while (cache_len < n && last_checked <= MAX_NUM) {
        idx_t cur = last_checked++;
        if (cur < 4)
            continue;

        uint32_t p = spf[cur];
        if (p == cur)                      /* cur is prime */
            continue;

        idx_t q = cur / p;
        if (q == 1 || spf[q] != q)         /* q not prime -> not semiprime */
            continue;

        seq_t sum = (seq_t)p + (seq_t)q;
        if ((sum & 1) == 0) {              /* keep only even sums */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache,
                                          cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = sum;
        }
    }
}

/* Public function: returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset start at 1 */
    ensure_cache(n);
    return (n <= cache_len) ? cache[n - 1] : 0;
}
