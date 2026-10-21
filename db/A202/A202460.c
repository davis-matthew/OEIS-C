/*
 * A202460  Number of (n+2) X 9 binary arrays with consecutive windows of
 *          three bits considered as a binary number nondecreasing in every
 *          row and column.
 * https://oeis.org/A202460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 252*a(n-5)
 *                 - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10)
 *
 * Initial values (n = 1 … 10):
 * a(1)=2744, a(2)=8064, a(3)=21972, a(4)=54618, a(5)=124740,
 * a(6)=264822, a(7)=528390, a(8)=1000023, a(9)=1808746, a(10)=3145568
 */

#include <stddef.h>

/* cache size: enough for n up to about 10000 – 20000 is safely above that */
#define CACHE_CAP 20000

static seq_t cache[CACHE_CAP + 1] = {0}; /* cache[0] unused */
static size_t max_computed = 0;

static const seq_t init_vals[11] = {
    0ULL,          /* 0 unused */
    2744ULL,       /* a(1)  */
    8064ULL,       /* a(2)  */
    21972ULL,      /* a(3)  */
    54618ULL,      /* a(4)  */
    124740ULL,     /* a(5)  */
    264822ULL,     /* a(6)  */
    528390ULL,     /* a(7)  */
    1000023ULL,    /* a(8)  */
    1808746ULL,    /* a(9)  */
    3145568ULL     /* a(10) */
};

static void compute_upto(idx_t n)
{
    if ((idx_t)max_computed >= n)
        return;

    for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i)
    {
        if (i <= 10)
        {
            cache[(size_t)i] = init_vals[(size_t)i];
        }
        else
        {
            seq_t pos = 0;
            seq_t neg = 0;
            pos += (seq_t)10  * cache[(size_t)(i - 1)];
            neg += (seq_t)45  * cache[(size_t)(i - 2)];
            pos += (seq_t)120 * cache[(size_t)(i - 3)];
            neg += (seq_t)210 * cache[(size_t)(i - 4)];
            pos += (seq_t)252 * cache[(size_t)(i - 5)];
            neg += (seq_t)210 * cache[(size_t)(i - 6)];
            pos += (seq_t)120 * cache[(size_t)(i - 7)];
            neg += (seq_t)45  * cache[(size_t)(i - 8)];
            pos += (seq_t)10  * cache[(size_t)(i - 9)];
            neg += (seq_t)1   * cache[(size_t)(i - 10)];
            cache[(size_t)i] = pos - neg;
        }
    }
    max_computed = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    compute_upto(n);
    return cache[(size_t)n];
}
