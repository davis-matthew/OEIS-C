/*
 * A228280  Number of nX5 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) follows a 13‑term linear recurrence
 *
 *   a(n) =  a(n-1) + 21·a(n-2) + 48·a(n-3) + 14·a(n-4)
 *            - 69·a(n-5)-38·a(n-6)+68·a(n-7)+13·a(n-8)
 *            - 57·a(n-9)+37·a(n-10)-8·a(n-11)-2·a(n-12)+a(n-13)
 *
 * Initial values (offset 1):
 *   a(1)=5,  a(2)=13,  a(3)=112,  a(4)=587,  a(5)=3631,
 *   a(6)=21166, a(7)=126119, a(8)=745178, a(9)=4416695,
 *   a(10)=26150120, a(11)=154877307, a(12)=917205757, a(13)=5431915952.
 *
 * The function memoises computed terms so that calls with any order of
 * indices run in O(n) time overall.  All arithmetic uses the type
 * seq_t; for unsigned seq_t the sign of the recurrence coefficients is
 * handled by separate positive and negative accumulators.
 */

#include <stdlib.h>

/* Memoisation storage – 0‑based indices (a(1) is stored at cache[0]) */
static seq_t  *cache   = NULL;
static size_t  cap     = 0;
static size_t  len     = 0;
static int     init    = 0;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need <= cap) return;
    size_t new_cap = cap ? cap : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise the first 13 terms of the sequence. */
static void init_table(void)
{
    if (init) return;
    ensure_capacity(13);
    cache[0]  = 5;            /* a(1) */
    cache[1]  = 13;           /* a(2) */
    cache[2]  = 112;          /* a(3) */
    cache[3]  = 587;          /* a(4) */
    cache[4]  = 3631;         /* a(5) */
    cache[5]  = 21166;        /* a(6) */
    cache[6]  = 126119;       /* a(7) */
    cache[7]  = 745178;       /* a(8) */
    cache[8]  = 4416695;      /* a(9) */
    cache[9]  = 26150120;     /* a(10) */
    cache[10] = 154877307;    /* a(11) */
    cache[11] = 917205757;    /* a(12) */
    cache[12] = 5431915952;   /* a(13) */
    len   = 13;
    init  = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    init_table();

    if ((size_t)n <= len) return cache[n-1];

    /* Compute remaining terms up to n */
    ensure_capacity(n);

    /* Recurrence coefficients for a(n-1) ... a(n-13) */
    static const int coeff[13] = {
        1,   21,   48,   14,  -69,  -38,  68,  13, -57,  37,  -8,  -2,  1
    };

    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t pos = 0;       /* sum of positive contributions */
        seq_t neg = 0;       /* sum of negative contributions (stored as positive) */

        /* a(i) depends on the previous 13 terms */
        for (int j = 0; j < 13; ++j) {
            seq_t term = cache[(i-2) - j];           /* cache stores a(i-j-1) at index (i-j-2) */
            int   c    = coeff[j];
            if (c > 0) {
                pos += term * (unsigned)c;
            } else if (c < 0) {
                neg += term * (unsigned)(-c);
            }
        }

        /* Result is guaranteed non‑negative */
        cache[i-1] = pos - neg;
    }

    len = n;
    return cache[n-1];
}
