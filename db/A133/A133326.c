/*
 * A133326  Numbers n such that 2*n^2 + 41 is a square.
 * https://oeis.org/A133326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n such that 2*n^2 + 41 is a perfect square.
   The odd‑indexed and even‑indexed subsequences satisfy the same
   linear recurrence of order two:
       a(1)=2, a(3)=20;  a(2k+1)=6*a(2k-1)-a(2k-3)   (k≥1)
       a(2)=8, a(4)=50;  a(2k)=6*a(2k-2)-a(2k-4)     (k≥2)
*/

#include <stddef.h>   /* for size_t */

#define MAX_N   10000U                     /* maximum n requested   */
#define SUB_SIZE ((MAX_N/2)+3U)            /* subsequence capacity  */

/* Caching of the two subsequences                                  */
static seq_t cacheA[SUB_SIZE];              /* odd indices          */
static seq_t cacheB[SUB_SIZE];              /* even indices         */
static size_t sizeA = 0;                    /* number of elements   */
static size_t sizeB = 0;

static void init_A(void)
{
    cacheA[0] = (seq_t)2;      /* a(1)   */
    cacheA[1] = (seq_t)20;     /* a(3)   */
    sizeA = 2;
}

static void init_B(void)
{
    cacheB[0] = (seq_t)8;      /* a(2)   */
    cacheB[1] = (seq_t)50;     /* a(4)   */
    sizeB = 2;
}

static void ensure_A(size_t k)
{
    if (sizeA == 0) init_A();
    while (sizeA <= k)
    {
        cacheA[sizeA] = 6 * cacheA[sizeA-1] - cacheA[sizeA-2];
        ++sizeA;
    }
}

static void ensure_B(size_t k)
{
    if (sizeB == 0) init_B();
    while (sizeB <= k)
    {
        cacheB[sizeB] = 6 * cacheB[sizeB-1] - cacheB[sizeB-2];
        ++sizeB;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* out of range, harmless */

    /* (n-1)/2 gives the index inside the corresponding subsequence   */
    const size_t k = (size_t)((n - 1U) / 2U);

    if (n & 1U) {                              /* odd n → cacheA   */
        ensure_A(k);
        return cacheA[k];
    } else {                                   /* even n → cacheB */
        ensure_B(k);
        return cacheB[k];
    }
}
