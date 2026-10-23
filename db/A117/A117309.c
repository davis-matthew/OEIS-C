/*
 * A117309  Triangular numbers for which the sum of the digits is a hexagonal
 *          number.
 * https://oeis.org/A117309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th triangular number whose digit sum is a hexagonal
 *            number
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers ---------- */

/* digit sum of a seq_t value */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* pre‑compute all hexagonal numbers up to a limit that covers any possible small digit sum */
#define HEX_MAX_SUM   200
static bool hex_check[HEX_MAX_SUM + 1] = {false};
static bool hex_inited = false;

static void init_hex(void)
{
    if (hex_inited) return;
    for (unsigned m = 0; ; ++m) {
        unsigned h = m * (2 * m - 1);   /* hexagonal n = m*(2m-1), m=0 gives 0 */
        if (h > HEX_MAX_SUM) break;
        hex_check[h] = true;
    }
    hex_inited = true;
}

/* ---------- cache handling ---------- */

static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  tri_k     = 0;          /* next triangular index to test */

/* enlarge the cache if necessary; cast the result of realloc to seq_t* for C++ */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache      = tmp;
    cache_cap  = new_cap;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    init_hex();                     /* ensure hexagonal lookup table exists */

    /* generate terms until we have enough for index n */
    while (cache_len <= (size_t)n) {
        /* triangular number t = k(k+1)/2 */
        seq_t t = (seq_t)tri_k * ((seq_t)tri_k + 1) / 2;

        /* check its digit sum */
        seq_t ds = digit_sum(t);
        bool ok = (ds <= HEX_MAX_SUM) && hex_check[(unsigned)ds];

        if (ok) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = t;
        }
        ++tri_k;
    }
    return cache[(size_t)n];
}
