/*
 * A236775  Number of (n+1)X(1+1) 0..2 arrays colored with the maximum plus
 *          the upper median plus the minimum minus the lower median of every
 *          2X2 subblock.
 * https://oeis.org/A236775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) =
 *            40*a(n-3)+528*a(n-4)+1464*a(n-5)+4428*a(n-6)+8904*a(n-7)
 *            -696*a(n-8)-16096*a(n-9)-77216*a(n-10)-159008*a(n-11)
 *            -61184*a(n-12)-18816*a(n-13)+190976*a(n-14)
 *            +365056*a(n-15)+172032*a(n-16)+73728*a(n-17) initial terms:
 *            a(1)=81, a(2)=474, a(3)=2797, a(4)=16360, a(5)=95961,...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t computed = 0; /* highest index computed so far */

/* 1-based array of initial terms up to a(17) */
static const seq_t init[18] = {
    0,                     /* unused 0 */
    81ULL,                 /* a(1) */
    474ULL,                /* a(2) */
    2797ULL,               /* a(3) */
    16360ULL,              /* a(4) */
    95961ULL,              /* a(5) */
    562284ULL,             /* a(6) */
    3296708ULL,            /* a(7) */
    19317952ULL,           /* a(8) */
    113216324ULL,          /* a(9) */
    663531280ULL,          /* a(10) */
    3889040836ULL,         /* a(11) */
    22792434328ULL,        /* a(12) */
    133579200184ULL,       /* a(13) */
    782877551360ULL,       /* a(14) */
    4588334140336ULL,      /* a(15) */
    26891207676512ULL,     /* a(16) */
    157602635364752ULL     /* a(17) */
};

/* coefficients for a(n), indexed by offset j (3..17) */
static const int coeff[18] = {
    0,      /* 0 */
    0,      /* 1 */
    0,      /* 2 */
    40,     /* 3  */
    528,    /* 4  */
    1464,   /* 5  */
    4428,   /* 6  */
    8904,   /* 7  */
    -696,   /* 8  */
    -16096, /* 9  */
    -77216, /* 10 */
    -159008,/* 11 */
    -61184, /* 12 */
    -18816, /* 13 */
    190976, /* 14 */
    365056, /* 15 */
    172032, /* 16 */
    73728   /* 17 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range, but callers assume n>=1 */

    /* Grow cache to accommodate n */
    if (cache == NULL || (idx_t) (sizeof(seq_t) * (n + 1)) > 0) {
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        } else if (n + 1 > computed) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
            if (tmp) cache = tmp;
        }
    }
    if (cache == NULL) return 0; /* allocation failure */

    /* Initialize cache with initial values if needed */
    if (computed < 17) {
        idx_t limit = (n < 17) ? n : 17;
        for (idx_t i = computed + 1; i <= limit; ++i) {
            cache[i] = init[i];
        }
        computed = (computed < 17) ? limit : computed;
    }

    /* Compute further terms using recurrence */
    if (n > computed) {
        for (idx_t i = (computed + 1 > 17 ? computed + 1 : 18); i <= n; ++i) {
            seq_t result = 0;
            /* Add positive coefficient contributions */
            for (int j = 3; j <= 17; ++j) {
                if (coeff[j] > 0) {
                    result += (seq_t)coeff[j] * cache[i - j];
                }
            }
            /* Subtract negative coefficient contributions */
            for (int j = 3; j <= 17; ++j) {
                if (coeff[j] < 0) {
                    result -= (seq_t)(-coeff[j]) * cache[i - j];
                }
            }
            cache[i] = result;
        }
        computed = n;
    }

    return cache[n];
}
