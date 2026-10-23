/*
 * A131955  a(n) = sum of digits of (n!! + a(n-1)).
 * https://oeis.org/A131955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = sum of digits of (n!! + a(n-1)) a(1) = 1
 * Range:     u8 n <= 75, u16 n <= 8300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- double‑factorial tables ---------- */
#define BASE         1000000000U           /* 1e9 (fits in 32‑bit unsigned) */
#define MAX_DIGITS   2100                  /* enough for n!! up to ~10 000 */

static uint32_t fact_digits[2][MAX_DIGITS];
static size_t   fact_len[2];   /* number of used limbs per parity */
static idx_t    fact_last[2];  /* largest n already computed per parity */
static int      fact_initialized = 0;

static void fact_init(void)
{
    if (fact_initialized) return;
    fact_digits[0][0] = 1;          /* 0!! */
    fact_len[0] = 1;
    fact_last[0] = 0;

    fact_digits[1][0] = 1;          /* 1!! */
    fact_len[1] = 1;
    fact_last[1] = 1;
    fact_initialized = 1;
}

/* multiply the stored double‑factorial by successive odd/even integers
 * up to the requested n (inclusive).  Only the parity chain containing n
 * is modified.
 */
static void ensure_factorial(idx_t n)
{
    int p = n & 1;                 /* parity chain: 0 = even, 1 = odd */
    idx_t last = fact_last[p];
    size_t len   = fact_len[p];
    uint32_t *d  = fact_digits[p];

    for (idx_t k = last + 2; k <= n; k += 2) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)d[i] * (uint64_t)k + carry;
            d[i] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        while (carry) {
            d[len++] = (uint32_t)(carry % BASE);
            carry   /= BASE;
        }
    }
    fact_len[p]   = len;
    fact_last[p]  = n;
}

/* ---------- cache for a(n) ----------------- */
static unsigned int *a_cache  = NULL;   /* a_cache[0] unused, a_cache[1] = 1 */
static size_t        a_cache_len = 0;   /* current allocated capacity */

static void cache_ensure(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    if ((size_t)n < a_cache_len) return;    /* already large enough */

    unsigned int *tmp = (unsigned int*)realloc(a_cache,
                                               needed * sizeof(unsigned int));
    if (!tmp) return;                      /* allocation failure – ignore */
    a_cache = tmp;

    size_t old_cap = a_cache_len;
    a_cache_len = needed;

    /* zero new indices */
    for (size_t i = old_cap; i < a_cache_len; ++i) a_cache[i] = 0;

    /* initialise a(1) once if not already set */
    if (old_cap == 0 && a_cache_len >= 2) {
        a_cache[0] = 0;          /* unused but keep defined */
        a_cache[1] = 1;          /* a(1) = 1 */
    }
}

/* ---------- main function ------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* below offset – undefined */

    /* initialise factorial tables once */
    fact_init();

    /* ensure cache can hold index n */
    cache_ensure(n);

    /* if already computed, return it */
    if (a_cache[n]) return (seq_t)a_cache[n];

    /* compute all missing values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (a_cache[i]) continue;     /* already known */

        /* update double‑factorial for i */
        ensure_factorial(i);

        int p = i & 1;
        size_t len = fact_len[p];
        uint32_t *d  = fact_digits[p];

        /* copy current double‑factorial into a temporary array */
        uint32_t tmp[MAX_DIGITS];
        for (size_t j = 0; j < len; ++j) tmp[j] = d[j];
        size_t tmp_len = len;

        /* add a(i‑1) to the temporary array */
        uint64_t carry = (uint64_t)a_cache[i - 1];
        size_t idx = 0;
        while (carry) {
            if (idx < tmp_len) {
                uint64_t sum = (uint64_t)tmp[idx] + carry;
                tmp[idx] = (uint32_t)(sum % BASE);
                carry   = sum / BASE;
            } else {
                tmp[tmp_len++] = (uint32_t)(carry % BASE);
                carry          /= BASE;
            }
            ++idx;
        }

        /* sum the decimal digits of the updated number */
        unsigned int sum = 0;
        for (size_t j = 0; j < tmp_len; ++j) {
            uint32_t v = tmp[j];
            if (!v) continue;
            while (v) {
                sum += v % 10;
                v   /= 10;
            }
        }

        a_cache[i] = sum;
    }

    return (seq_t)a_cache[n];
}
