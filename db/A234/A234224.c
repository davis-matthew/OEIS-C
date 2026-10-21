/*
 * A234224  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 10*a(n-1) +16*a(n-2) -460*a(n-3) +789*a(n-4)
 *            +7170*a(n-5) * -23944*a(n-6) -39920*a(n-7) +246076*a(n-8) *
 *            -38440*a(n-9) -1115136*a(n-10) +1151040*a(n-11) *
 *            +2005056*a(n-12) -3657600*a(n-13) -331776*a(n-14) *
 *            +3456000*a(n-15) -1658880*a(n-16). * The first 16 terms (offset
 *            1) are given below
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* provisionally store the first 16 terms; index 0 is unused */
static const seq_t initial[17] = {
    0ULL,             /* unused */
    32384ULL,         /* a(1) */
    76972ULL,         /* a(2) */
    188040ULL,        /* a(3) */
    503272ULL,        /* a(4) */
    1379816ULL,       /* a(5) */
    4055128ULL,       /* a(6) */
    12178344ULL,      /* a(7) */
    38671960ULL,      /* a(8) */
    125308904ULL,     /* a(9) */
    425135512ULL,     /* a(10) */
    1469047080ULL,    /* a(11) */
    5270085592ULL,    /* a(12) */
    19196627816ULL,   /* a(13) */
    72039007768ULL,   /* a(14) */
    273444136104ULL,  /* a(15) */
    1062448914520ULL  /* a(16) */
};

/* recurrence coefficients: c[0] = 10, c[1] = 16, ... , c[15] = -1658880   */
static const int coeff[16] = {
    10,      16,    -460,    789,    7170,
    -23944,   -39920, 246076, -38440, -1115136,
    1151040, 2005056, -3657600, -331776, 3456000,
    -1658880
};

/* buffer large enough for the expected maximum n.
   20000 is well above the 10000 mentioned in the requirements. */
#define CACHE_SIZE 20001
static seq_t cache[CACHE_SIZE];
static idx_t cache_len = 0;   /* highest index that has been computed */

/* initialise the cache with the first 16 terms if needed */
static void
ensure_initialised(void)
{
    if (cache_len != 0)
        return;
    /* fill the first 16 positions; cache[0] is unused */
    for (int i = 1; i <= 16; ++i)
        cache[i] = initial[i];
    cache_len = 16;
}

/* compute a(n) for arbitrary n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)          /* below offset – undefined, return 0 */
        return 0;

    if (n >= CACHE_SIZE) /* safety guard – if too large, stop computing */
        n = CACHE_SIZE - 1;

    ensure_initialised();

    if (n > cache_len) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t sum = 0;
            for (int j = 0; j < 16; ++j) {
                /* coeff[j] may be negative; cast to seq_t for multiplication */
                sum += (seq_t)coeff[j] * cache[k - j - 1];
            }
            cache[k] = sum;
        }
        cache_len = n;
    }

    return cache[n];
}
