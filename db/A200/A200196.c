/*
 * A200196  Number of -n..n arrays x(0..6) of 7 elements with zero sum,
 *          adjacent elements differing by more than one, and elements
 *          alternately increasing and decreasing.
 * https://oeis.org/A200196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-5) + a(n-6) - 2*a(n-7) + 2*a(n-8)
 *            + a(n-9) - a(n-13) - 2*a(n-14) + 2*a(n-15) - a(n-16) + a(n-17) +
 *            a(n-19) - 2*a(n-21) + a(n-22) (valid for n > 22) Initial values:
 *            a(1)=0, a(2)=104, a(3)=1534, a(4)=9856, a(5)=40632, a(6)=127800,
 *            a(7)=334278, a(8)=765598, a(9)=158541...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 32, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *memo = NULL;
static size_t memo_size = 0;
static idx_t max_computed = 0;

static const seq_t init[23] = {
    0ULL,                 /* dummy 0, unused */
    0ULL,                 /* a(1)  */
    104ULL,                /* a(2)  */
    1534ULL,               /* a(3)  */
    9856ULL,               /* a(4)  */
    40632ULL,              /* a(5)  */
    127800ULL,             /* a(6)  */
    334278ULL,             /* a(7)  */
    765598ULL,             /* a(8)  */
    1585416ULL,            /* a(9)  */
    3034396ULL,            /* a(10) */
    5450616ULL,            /* a(11) */
    9293800ULL,            /* a(12) */
    15170448ULL,           /* a(13) */
    23863548ULL,           /* a(14) */
    36362438ULL,           /* a(15) */
    53897896ULL,           /* a(16) */
    77976766ULL,           /* a(17) */
    110422568ULL,          /* a(18) */
    153414642ULL,          /* a(19) */
    209534448ULL,          /* a(20) */
    281809258ULL,          /* a(21) */
    373763982ULL           /* a(22) */
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n <= 22) return init[n];

    /* Allocate or resize memo array if necessary */
    if (memo == NULL) {
        memo_size = (n > 22 ? n : 22) + 1;          /* indices 0..n inclusive */
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_size);
        if (!memo) return (seq_t)0;                 /* allocation failure */
        for (idx_t i = 0; i < memo_size; ++i) memo[i] = 0;
        for (idx_t i = 1; i <= 22; ++i) memo[i] = init[i];
        max_computed = 22;
    } else if (n >= memo_size) {                    /* grow array */
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;
        for (size_t i = memo_size; i < new_size; ++i) tmp[i] = 0;
        memo = tmp;
        memo_size = new_size;
    }

    if (n <= max_computed) return memo[n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* sum of positive contributions */
        seq_t pos = 0;
        pos += memo[i-1];
        pos += memo[i-1];            /* +2*a(i-1) */
        pos += memo[i-6];
        pos += memo[i-8];
        pos += memo[i-8];            /* +2*a(i-8) */
        pos += memo[i-9];
        pos += memo[i-15];
        pos += memo[i-15];           /* +2*a(i-15) */
        pos += memo[i-17];
        pos += memo[i-19];
        pos += memo[i-22];

        /* sum of negative contributions */
        seq_t neg = 0;
        neg += memo[i-3];
        neg += memo[i-5];
        neg += memo[i-7];
        neg += memo[i-7];            /* -2*a(i-7) */
        neg += memo[i-13];
        neg += memo[i-14];
        neg += memo[i-14];           /* -2*a(i-14) */
        neg += memo[i-16];
        neg += memo[i-21];
        neg += memo[i-21];           /* -2*a(i-21) */

        memo[i] = pos - neg;
    }

    max_computed = n;
    return memo[n];
}
