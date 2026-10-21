/*
 * A208460  Triangle read by rows: T(n,k) = n minus the k-th proper divisor of
 *          n.
 * https://oeis.org/A208460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n - d, where d is the k‑th proper divisor of n (the
 *            sequence is obtained by flattening the triangle T(n,k)=n-d);
 *            a(2) is the first term in the printed list
 * Range:     u8 n <= 1211, u16 n <= 9081, u32 n <= 9081, u64 n <= 9081,
 *            u128 n <= 9081, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of helper function. */
static void _gen_divs(size_t idx, idx_t cur,
                      const uint32_t *p, const uint32_t *e, size_t pc,
                      idx_t *divs, size_t *cnt);
static int cmp_idx_t(const void *a, const void *b);

/* Precomputation parameters */
#define PRECALC_LIMIT 200000           /* enough for all requested n   */
static idx_t *cumProper = NULL;        /* cumulative proper divisor counts */
static uint32_t *spf = NULL;           /* smallest prime factor table */

/* One‑time initialisation */
static void _init(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    idx_t limit = PRECALC_LIMIT;

    /* Allocate Sieve of Eratosthenes and cumulated counts */
    spf      = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    cumProper = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));

    /* Build spf table via linear sieve. */
    size_t primeCap = (size_t)(limit / 10 + 10);
    uint32_t *primeList = (uint32_t *)malloc(primeCap * sizeof(uint32_t));
    size_t primeCount = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primeCount < primeCap)
                primeList[primeCount++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primeCount; ++j) {
            uint32_t p = primeList[j];
            idx_t val = i * p;
            if (val > limit) break;
            spf[val] = p;
            if (p == spf[i]) break;
        }
    }

    /* Compute cumulated proper divisor counts. */
    cumProper[0] = 0;
    cumProper[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t temp = i;
        idx_t dCount = 1;
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            dCount *= (exp + 1);
        }
        idx_t properCnt = dCount - 1;
        cumProper[i] = cumProper[i - 1] + properCnt;
    }

    free(primeList);
}

/* Recursive generation of all divisors of a number. */
static void _gen_divs(size_t idx, idx_t cur,
                      const uint32_t *p, const uint32_t *e,
                      size_t pc, idx_t *divs, size_t *cnt)
{
    if (idx == pc) {
        divs[(*cnt)++] = cur;
        return;
    }
    idx_t val = cur;
    for (uint32_t i = 0; i <= e[idx]; ++i) {
        _gen_divs(idx + 1, val, p, e, pc, divs, cnt);
        val *= p[idx];
    }
}

/* Comparison function for qsort. */
static int cmp_idx_t(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* behavior below offset ignored        */
    _init();                       /* ensure tables are ready exactly once */

    /* posIndex = 0‑based position in the flattened triangle. */
    idx_t posIndex = n - 1;

    /* Find row r such that cumProper[r-1] < posIndex <= cumProper[r]. */
    idx_t low = 2, high = PRECALC_LIMIT, r = PRECALC_LIMIT;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (cumProper[mid] < posIndex) {
            low = mid + 1;
        } else {
            r = mid;
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t prevCum    = cumProper[r - 1];
    idx_t posInRow   = posIndex - prevCum;   /* 1‑based within this row */

    /* Factorise r using spf. */
    idx_t tmp = r;
    uint32_t facPrimes[16];
    uint32_t facExp[16];
    size_t facCnt = 0;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint32_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        facPrimes[facCnt] = p;
        facExp[facCnt]    = exp;
        ++facCnt;
    }

    /* Generate all divisors of r. */
    idx_t allDiv[1024];
    size_t allCnt = 0;
    _gen_divs(0, 1, facPrimes, facExp, facCnt, allDiv, &allCnt);

    /* Filter proper divisors (exclude r itself). */
    idx_t properDiv[1024];
    size_t properCnt = 0;
    for (size_t i = 0; i < allCnt; ++i) {
        if (allDiv[i] != r) {
            properDiv[properCnt++] = allDiv[i];
        }
    }

    /* Sort proper divisors ascending. */
    qsort(properDiv, properCnt, sizeof(idx_t), cmp_idx_t);

    /* Retrieve the divisor at position `posInRow`. */
    idx_t divisor = properDiv[posInRow - 1];
    seq_t result = (seq_t)r - (seq_t)divisor;
    return result;
}
