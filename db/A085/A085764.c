/*
 * A085764  Least integer m such that between m and 2m there are n
 *          palindromes.
 * https://oeis.org/A085764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lower_bound, upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements the OEIS definition of A085764:
 *  a(n) = the least integer m such that the interval [m,2m] contains
 *  exactly n decimal palindromes (including m and 2m). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lower_bound.h"
#include "../utils/upper_bound.h"

/* ------------------------------------------------------------- */
/*  Palindrome generation utilities                              */
/* ------------------------------------------------------------- */
#define LIMIT ((seq_t)2000000000ULL)          /* upper bound for 2*m   */
#define PAL_CAP (250000)                      /* capacity for palindromes */

static bool pal_init = false;
static seq_t pal[PAL_CAP];
static size_t pal_cnt = 0;

/* powers of ten up to 20: pow10[i] = 10^i */
static seq_t pow10[20];
static void init_pow10(void)
{
    pow10[0] = 1;
    for (int i = 1; i < 20; ++i)
        pow10[i] = pow10[i - 1] * 10;
    pal_init = true;
}

/* generate all decimal palindromes <= LIMIT, store in pal[] */
static void generate_palindromes(void)
{
    if (!pal_init) init_pow10();

    /* number of digits in LIMIT */
    int maxlen = 1;
    seq_t tmp = LIMIT;
    while (tmp >= 10) {
        tmp /= 10;
        maxlen++;
    }

    size_t cnt = 0;
    for (int len = 1; len <= maxlen && cnt < PAL_CAP; ++len) {
        int half_len = (len + 1) / 2;
        seq_t start = pow10[half_len - 1];
        seq_t end   = pow10[half_len] - 1;
        if (len == 1)          /* single-digit palindromes start at 1 */
            start = 1;

        for (seq_t half = start; half <= end && cnt < PAL_CAP; ++half) {
            seq_t palin = half;
            seq_t rev = half;
            if (len & 1)         /* odd length: skip the middle digit */
                rev /= 10;
            while (rev > 0) {
                palin = palin * 10 + (rev % 10);
                rev /= 10;
            }
            if (palin > LIMIT) break;
            pal[cnt++] = palin;
        }
    }
    pal_cnt = cnt;
}

/* ------------------------------------------------------------- */
/*  Binary search helpers                                        */

/* ------------------------------------------------------------- */
/*  Main sequence function                                      */
/* ------------------------------------------------------------- */
#define ANS_CAP 200000
static seq_t ans[ANS_CAP + 1];
static idx_t ans_provided = 0;          /* highest n with ans[n] known */
static size_t last_m_checked = 0;       /* max m processed so far */

/* a(n) : least m such that [m,2m] contains exactly n palindromes */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, though offset starts at 1 */

    /* lazy initialization of palindromes and answer table */
    static bool init = false;
    if (!init) {
        init = true;
        /* initialise answer array */
        for (size_t i = 1; i <= ANS_CAP; ++i)
            ans[i] = 0;
        generate_palindromes();
    }

    /* if already computed, return */
    if (n <= ans_provided && ans[n] != 0)
        return ans[n];

    /* search further until this n is filled */
    for (size_t m = last_m_checked + 1; ; ++m) {
        last_m_checked = m;
        seq_t low = (seq_t)m;
        seq_t high = low * 2;

        size_t lo = lower_bound(pal, pal_cnt, low);
        size_t hi = upper_bound(pal, pal_cnt, high);
        size_t cnt = hi - lo;      /* number of palindromes in [m,2m] */

        if (cnt <= ANS_CAP && ans[cnt] == 0) {
            ans[cnt] = low;         /* first time this exact count occurs */
            if (cnt > ans_provided)
                ans_provided = cnt;
        }

        if (cnt == (size_t)n)
            return ans[n];
        /* Continue until n is found. For safety, protect infinite loop */
        if (m == SIZE_MAX - 1) break;
    }
    /* Should never reach here */
    return 0;
}
