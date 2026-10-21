/*
 * A059524  Number of nonzero 4 X n binary arrays with all 1's connected.
 * https://oeis.org/A059524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 17*a(n-1) - 90*a(n-2) + 230*a(n-3) - 272*a(n-4) - 75*a(n-5)
 *               + 623*a(n-6) - 632*a(n-7) + 65*a(n-8) + 255*a(n-9) - 198*a(n-10)
 *               + 162*a(n-11) - 96*a(n-12) + 11*a(n-13) + a(n-14)
 * Initial values:
 * a(0)=0, a(1)=10, a(2)=108, a(3)=1126, a(4)=11506, a(5)=116166,
 * a(6)=1168586, a(7)=11749134, a(8)=118127408, a(9)=1187692422,
 * a(10)=11941503498, a(11)=120064335342, a(12)=1207171430452,
 * a(13)=12137349489598 */
#include <stdlib.h>

static const int coeff[14] = {
    17,  -90,   230,  -272,  -75,   623,  -632,   65,
    255, -198,  162,  -96,   11,    1
};

static const seq_t init[14] = {
    (seq_t)0ULL,    (seq_t)10ULL,   (seq_t)108ULL,          (seq_t)1126ULL,
    (seq_t)11506ULL, (seq_t)116166ULL, (seq_t)1168586ULL,      (seq_t)11749134ULL,
    (seq_t)118127408ULL, (seq_t)1187692422ULL,          (seq_t)11941503498ULL,
    (seq_t)120064335342ULL, (seq_t)1207171430452ULL,   (seq_t)12137349489598ULL
};

static seq_t *memo = NULL;
static size_t memo_len = 0;
static size_t memo_cap = 0;

/* Extend memo up to index n (inclusive). */
static void ensure(size_t n)
{
    if (memo == NULL) {
        memo_cap = (n < 16 ? 16 : n + 1);
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) exit(1);
        for (size_t i = 0; i < 14; ++i) memo[i] = init[i];
        memo_len = 14;
    }

    while (memo_len <= n) {
        if (memo_len >= memo_cap) {
            memo_cap = memo_cap * 2;
            seq_t *tmp = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
            if (!tmp) { free(memo); exit(1); }
            memo = tmp;
        }
        /* Compute a[memo_len] from the recurrence. */
        size_t i = memo_len;
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 14; ++k) {
            seq_t t = memo[i - 1 - k];
            int c = coeff[k];
            if (c >= 0)
                pos += t * (seq_t)c;
            else
                neg += t * (seq_t)(-c);
        }
        memo[i] = pos - neg;
        ++memo_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 14) return init[(size_t)n];
    ensure((size_t)n);
    return memo[(size_t)n];
}
