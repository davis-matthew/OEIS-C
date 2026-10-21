/*
 * A057421  Consider the solutions to k = a+b = x*y and a*b = k*(x+y) where k,
 *          a, b, x, and y are all positive integers, ordered by increasing k
 *          and, in case of ties, by increasing x. Sequence gives values of
 *          a*b.
 * https://oeis.org/A057421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057421: solutions of k = a + b = x·y and a·b = k·(x + y),
   ordered by increasing k and, for ties, by increasing x.  The function
   returns a·b for the n‑th solution (1‑indexed). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer square root of a 128‑bit unsigned integer using binary search. */
static inline unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned long long low = 0, high = ~0ULL;          /* 2^64‑1 */
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Smallest prime factor table (sieve).  It is rebuilt when a larger bound
   is required; for the limits needed it is cheap and fast. */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

static void ensure_spf(size_t m)
{
    if (m <= spf_size) return;
    spf = (uint32_t *)realloc(spf, m * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    if (m > 0) spf[0] = 0;
    if (m > 1) spf[1] = 1;
    for (size_t i = 2; i < m; ++i) spf[i] = 0;
    for (size_t i = 2; i < m; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i < m) {
                for (uint64_t j = (uint64_t)i * i; j < m; j += i)
                    if (spf[(size_t)j] == 0) spf[(size_t)j] = (uint32_t)i;
            }
        }
    }
    spf_size = m;
}

/* Factorise n using the spf table.  pr[] and exp[] must be large enough. */
static void factorize(uint32_t n, uint32_t *pr, int *exp, int *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        pr[(*cnt)] = p;
        exp[(*cnt)] = e;
        ++(*cnt);
    }
}

/* Recursive generation of all divisors of a number with given prime factors. */
static void gen_divs_rec(const uint32_t *pr, const int *exp, int cnt,
                         int idx, uint64_t cur, uint64_t *divs, int *dcnt)
{
    if (idx == cnt) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t base = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, cnt, idx + 1, cur * base, divs, dcnt);
        base *= pr[idx];
    }
}

/* Compare two unsigned 64‑bit integers for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑indexed; 0 is unused */

    /* Scratch space for factorisation. */
    static uint32_t *pr = NULL;
    static int       *exp = NULL;
    static size_t    cap = 0;
    if (cap < 32) {
        cap = 32;
        pr  = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        exp = (int *)realloc(exp, cap * sizeof(int));
        if (!pr || !exp) exit(EXIT_FAILURE);
    }

    size_t found = 0;
    uint64_t current_k = 0;

    while (found < n) {
        ++current_k;                         /* next k */
        ensure_spf((size_t)current_k + 1);    /* need spf for current_k anyway */

        /* Factorise current_k. */
        int fac_cnt;
        factorize((uint32_t)current_k, pr, exp, &fac_cnt);

        /* Generate all divisors of current_k. */
        uint64_t divs[4096];
        int dcnt = 0;
        gen_divs_rec(pr, exp, fac_cnt, 0, 1, divs, &dcnt);

        /* Sort divisors so that we can stop after x > k/x. */
        qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

        for (int i = 0; i < dcnt; ++i) {
            uint64_t x = divs[i];
            uint64_t y = current_k / x;
            if (x > y) break;                /* ensure x <= y */

            /* Compute discriminant condition. */
            int64_t diff = (int64_t)current_k - 4LL * (int64_t)(x + y);
            if (diff < 0) continue;

            unsigned __int128 D = (unsigned __int128)current_k *
                                  (unsigned __int128)(uint64_t)diff;
            unsigned long long sqrtD = isqrt_u128(D);
            if ((unsigned __int128)sqrtD * sqrtD != D) continue;
            if (((uint64_t)current_k + sqrtD) & 1ULL) continue;   /* parity check */

            /* Valid solution found.  Return product a*b = k·(x+y). */
            seq_t product = (seq_t)current_k *
                            (seq_t)(x + (current_k / x));
            ++found;
            if (found == n) return product;
        }
    }
    /* Control should never reach here. */
    return 0;
}
