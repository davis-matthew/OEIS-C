/*
 * A000101  Record gaps between primes (upper end) (compare A002386, which
 *          gives lower ends of these gaps).
 * https://oeis.org/A000101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A002386(n) + A005250(n). Terms below 2^25 come from a
 *            prime sieve that records each new maximal gap. Larger terms come
 *            from the published record-gap table (lower end + gap); terms
 *            above 2^64 are stored as decimal halves, hi*10^10 + lo
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 34, u64 n <= 80, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A101_LIM (1u << 25)
#define A101_NSIEVE 24

static uint64_t a101_small[64];
static int a101_nsmall = -1;

/* A002386(n), A005250(n) for n = 25 .. 80 */
static const uint64_t a101_lower[] = {
    47326693ULL, 122164747ULL, 189695659ULL, 191912783ULL, 387096133ULL,
    436273009ULL, 1294268491ULL, 1453168141ULL, 2300942549ULL, 3842610773ULL,
    4302407359ULL, 10726904659ULL, 20678048297ULL, 22367084959ULL,
    25056082087ULL, 42652618343ULL, 127976334671ULL, 182226896239ULL,
    241160624143ULL, 297501075799ULL, 303371455241ULL, 304599508537ULL,
    416608695821ULL, 461690510011ULL, 614487453523ULL, 738832927927ULL,
    1346294310749ULL, 1408695493609ULL, 1968188556461ULL, 2614941710599ULL,
    7177162611713ULL, 13829048559701ULL, 19581334192423ULL,
    42842283925351ULL, 90874329411493ULL, 171231342420521ULL,
    218209405436543ULL, 1189459969825483ULL, 1686994940955803ULL,
    1693182318746371ULL, 43841547845541059ULL, 55350776431903243ULL,
    80873624627234849ULL, 203986478517455989ULL, 218034721194214273ULL,
    305405826521087869ULL, 352521223451364323ULL, 401429925999153707ULL,
    418032645936712127ULL, 804212830686677669ULL, 1425172824437699411ULL,
    5733241593241196731ULL, 6787988999657777797ULL,
    15570628755536096243ULL, 17678654157568189057ULL,
    18361375334787046697ULL
};
static const uint16_t a101_gap[] = {
    220, 222, 234, 248, 250, 282, 288, 292, 320, 336,
    354, 382, 384, 394, 456, 464, 468, 474, 486, 490,
    500, 514, 516, 532, 534, 540, 582, 588, 602, 652,
    674, 716, 766, 778, 804, 806, 906, 916, 924, 1132,
    1184, 1198, 1220, 1224, 1248, 1272, 1328, 1356, 1370, 1442,
    1476, 1488, 1510, 1526, 1530, 1550
};

/* upper ends beyond 2^64 for n = 81 .. 85, as hi*10^10 + lo (0 = unknown) */
static const uint64_t a101_big_hi[] = {
    1847005794ULL, 1857167343ULL, 1858718743ULL, 0ULL, 10141231999ULL
};
static const uint64_t a101_big_lo[] = {
    6260699783ULL, 2051831671ULL, 4450230159ULL, 0ULL, 6363310923ULL
};

static void a101_init(void)
{
    uint32_t half = A101_LIM / 2, i, j;
    unsigned char *comp = (unsigned char *)calloc(half, 1);
    uint64_t last = 2, maxgap = 0;
    a101_nsmall = 0;
    if (!comp) return;
    /* index i represents the odd number 2i+1 */
    for (i = 1; (uint64_t)(2 * i + 1) * (2 * i + 1) < A101_LIM; i++)
        if (!comp[i]) {
            uint32_t p = 2 * i + 1;
            for (j = (p * p) / 2; j < half; j += p) comp[j] = 1;
        }
    for (i = 1; i < half && a101_nsmall < A101_NSIEVE; i++) {
        if (!comp[i]) {
            uint64_t q = 2 * (uint64_t)i + 1;
            if (q - last > maxgap) {
                maxgap = q - last;
                a101_small[a101_nsmall++] = q;
            }
            last = q;
        }
    }
    free(comp);
}

seq_t a(idx_t n)
{
    size_t k, nlow = sizeof(a101_gap) / sizeof(a101_gap[0]);
    size_t nbig = sizeof(a101_big_hi) / sizeof(a101_big_hi[0]);
    seq_t r;
    if (n < 1) return (seq_t)0;
    if (a101_nsmall < 0) a101_init();
    if (n <= (idx_t)a101_nsmall) return (seq_t)a101_small[n - 1];
    if (n <= A101_NSIEVE) return (seq_t)0;
    k = (size_t)(n - 1 - A101_NSIEVE);
    if (k < nlow) {
        r = (seq_t)a101_lower[k];
        r += (seq_t)a101_gap[k];
        return r;
    }
    k -= nlow;
    if (k >= nbig) return (seq_t)0;
    r = (seq_t)a101_big_hi[k];
    r *= (seq_t)10000000000ULL;
    r += (seq_t)a101_big_lo[k];
    return r;
}
