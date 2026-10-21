/*
 * A085257  Number of ways to write n as sum of a powerful and a squarefree
 *          number.
 * https://oeis.org/A085257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a powerful number *
 *            plus a square‑free number. The function precomputes all *
 *            powerful numbers up to n‑1 and a table of square‑free * numbers
 *            up to n, then counts the valid decompositions
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/*  Meta data (global cache) */
static idx_t cached_limit = 0;
static uint8_t *sqfree = NULL;            /* 1 = square‑free, 0 = not */
static size_t sqfree_size = 0;
static idx_t *powerful = NULL;            /* list of powerful numbers <= limit */
static size_t powerful_cnt = 0;
static size_t powerful_cap = 0;

/*  Re‑allocate tables so that values up to N (inclusive) exist. */
static void ensure_up_to(idx_t N)
{
    if (N <= cached_limit)
        return;

    /* ---------- Square‑free table ---------- */
    if (sqfree == NULL) {
        sqfree = (uint8_t*)malloc(sizeof(uint8_t) * (N + 1));
        sqfree_size = N + 1;
    } else if (N + 1 > sqfree_size) {
        uint8_t *tmp = (uint8_t*)realloc(sqfree, sizeof(uint8_t) * (N + 1));
        sqfree = tmp;
        sqfree_size = N + 1;
    }

    /* fill with 1s (assume square‑free first) */
    for (idx_t i = 0; i <= N; ++i)
        sqfree[i] = 1;

    /* sieve multiples of squares */
    idx_t limit = (idx_t)sqrt((long double)N) + 1;
    uint8_t *isPrime = (uint8_t*)malloc(limit + 1);
    for (idx_t i = 0; i <= limit; ++i)
        isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (isPrime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                isPrime[q] = 0;
        }
    }
    for (idx_t p = 2; p <= limit; ++p) {
        if (isPrime[p]) {
            uint64_t sq = (uint64_t)p * p;
            for (uint64_t k = sq; k <= (uint64_t)N; k += sq)
                sqfree[(idx_t)k] = 0;
        }
    }
    free(isPrime);

    /* ---------- Powerful numbers table ---------- */
    free(powerful);
    powerful_cap = 128;
    powerful = (idx_t*)malloc(sizeof(idx_t) * powerful_cap);
    powerful_cnt = 0;

    for (idx_t a = 1; (uint64_t)a * a <= (uint64_t)N; ++a) {
        unsigned __int128 a2 = (unsigned __int128)a * a;
        for (idx_t b = 1; ; ++b) {
            if (b > N)  /* b^3 will exceed N for sure */
                break;
            if (!sqfree[b])
                continue;  /* only square‑free b allowed */

            unsigned __int128 b3 = (unsigned __int128)b * b * b;
            unsigned __int128 val = a2 * b3;
            if (val > (unsigned __int128)N)
                break;

            idx_t v = (idx_t)val;
            if (powerful_cnt == powerful_cap) {
                powerful_cap <<= 1;
                powerful = (idx_t*)realloc(powerful, sizeof(idx_t) * powerful_cap);
            }
            powerful[powerful_cnt++] = v;
        }
    }

    cached_limit = N;
}

/*  Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;              /* a(1) = 0 */

    /* we need square‑free numbers up to n, and powerful numbers < n */
    ensure_up_to(n);                  /* sqfree up to n, powerful up to n */

    seq_t cnt = 0;
    for (size_t i = 0; i < powerful_cnt; ++i) {
        idx_t p = powerful[i];
        if (p >= n)                  /* require sf >= 1 */
            continue;
        idx_t rem = n - p;
        if (sqfree[rem])
            ++cnt;
    }
    return cnt;
}
