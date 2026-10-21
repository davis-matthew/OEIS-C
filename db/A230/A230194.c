/*
 * A230194  Number of ways to write n = x + y + z (x, y, z > 0) such that all
 *          the 11 integers 6*x-1, 6*x+1, 6*x-5, 6*x+5, 6*y-1, 6*y-5, 6*y+5,
 *          6*(x+y)+5, 6*z-1, 6*z-5 and 6*z+5 are prime.
 * https://oeis.org/A230194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230194: a(n) = Number of ways to write n = x + y + z (x,y,z > 0) such that all
   11 integers 6*x-1, 6*x+1, 6*x-5, 6*x+5, 6*y-1, 6*y-5, 6*y+5, 6*(x+y)+5,
   6*z-1, 6*z-5 and 6*z+5 are prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Global storage for primes and helper arrays */
static char *primeFlag = NULL;          /* primeFlag[i] == 1 iff i is prime */
static uint64_t primeLimit = 0;         /* largest value with primeFlag defined */

static int  *XsetArr = NULL;           /* indices of x satisfying the x‑conditions */
static size_t XsetSize = 0;
static int  *YsetArr = NULL;           /* indices of y / z satisfying the y‑conditions */
static size_t YsetSize = 0;
static unsigned char *YflagArr = NULL;  /* YflagArr[i] == 1 iff i is in Yset */
static size_t YflagSize = 0;

/* Ensure that primeFlag[0..L] is defined and contains primality info. */
static void ensurePrimeLimit(uint64_t L)
{
    if (L <= primeLimit) return;
    /* Allocate new table and sieve */
    if (primeFlag) free(primeFlag);
    primeFlag = (char*)malloc(L + 1);
    if (!primeFlag) return;          /* allocation failed – program will crash later */
    memset(primeFlag, 1, L + 1);
    primeFlag[0] = primeFlag[1] = 0;
    uint64_t i, j;
    for (i = 2; i * i <= L; ++i) {
        if (primeFlag[i]) {
            for (j = i * i; j <= L; j += i) primeFlag[j] = 0;
        }
    }
    primeLimit = L;
}

/* Ensure auxiliary arrays are large enough */
static void ensureArraySize(size_t need)
{
    if (XsetArr == NULL || XsetSize < need) {
        if (XsetArr) free(XsetArr);
        XsetArr = (int*)malloc(need * sizeof(int));
        XsetSize = need;
    }
    if (YsetArr == NULL || YsetSize < need) {
        if (YsetArr) free(YsetArr);
        YsetArr = (int*)malloc(need * sizeof(int));
        YsetSize = need;
    }
    if (YflagArr == NULL || YflagSize < need) {
        if (YflagArr) free(YflagArr);
        YflagArr = (unsigned char*)malloc(need * sizeof(unsigned char));
        YflagSize = need;
    }
}

/* The required function for the sequence. */
seq_t a(idx_t n)
{
    if (n < 6) return 0;     /* a(1)…a(5) are zero */

    /* Determine the prime table limit: needed up to 6*n+5. */
    uint64_t limit = 6 * (uint64_t)n + 5;
    ensurePrimeLimit(limit);

    /* Ensure auxiliary arrays can hold up to n+1 elements. */
    size_t need = (size_t)n + 1;
    ensureArraySize(need);

    /* Build Yset and Xset, and a quick membership array for Yset. */
    size_t xCount = 0, yCount = 0;
    memset(YflagArr, 0, need * sizeof(unsigned char));

    for (size_t k = 1; k <= n; ++k) {
        uint64_t base = 6 * k - 1;          /* p = 6k-1 */
        if (base > primeLimit || !primeFlag[base]) continue;

        bool y_ok = false;
        uint64_t p_minus_4 = base >= 4 ? base - 4 : 0;
        if (p_minus_4 >= 2 && primeFlag[p_minus_4] && primeFlag[base + 6]) y_ok = true;

        if (!y_ok) continue;
        YsetArr[yCount++] = (int)k;
        YflagArr[k] = 1;

        if (primeFlag[6 * k + 1]) {
            XsetArr[xCount++] = (int)k;
        }
    }

    /* Count ordered triples (x, y, z). */
    seq_t count = 0;
    for (size_t ix = 0; ix < xCount; ++ix) {
        int x = XsetArr[ix];
        for (size_t iy = 0; iy < yCount; ++iy) {
            int y = YsetArr[iy];
            int sumxy = x + y;
            if (sumxy >= (int)n) continue;      /* need z > 0 */

            int z = (int)n - sumxy;
            if (!primeFlag[6 * sumxy + 5]) continue;

            if (YflagArr[z]) ++count;
        }
    }

    return count;
}
