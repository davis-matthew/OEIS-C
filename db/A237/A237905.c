/*
 * A237905  Number of (n+1)X(1+1) 0..3 arrays with the maximum minus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1884*a(n-4) +28048*a(n-6) +134800*a(n-8) +241600*a(n-10)
 *            * +86848*a(n-12) -80384*a(n-14) +25600*a(n-16) for n>17 *
 *            Initial values: * a(1)=256, a(2)=1780, a(3)=12552, a(4)=88544, *
 *            a(5)=635200, a(6)=4438816, a(7)=31507328, a(8)=221652256, *
 *            a(9)=1584432064, a(10)=11094868672, a(11)=78930109...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[19] = {
    0ULL,                      /* dummy 0 index */
    256ULL,
    1780ULL,
    12552ULL,
    88544ULL,
    635200ULL,
    4438816ULL,
    31507328ULL,
    221652256ULL,
    1584432064ULL,
    11094868672ULL,
    78930109696ULL,
    554462140416ULL,
    3957466858304ULL,
    27739531863040ULL,
    197546198927360ULL,
    1386754238612992ULL,
    9890947053918976ULL,
    69362350056729600ULL
};

/* Memoization buffer – sufficient for n up to about 10 000. */
static seq_t memo[10001];
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* Initialize memo table on first call. */
    if (!max_computed) {
        for (idx_t i = 1; i <= 18; ++i)
            memo[i] = init_vals[i];
        /* memo[0] remains default‑constructed (zero). */
        max_computed = 18;
    }

    if (n <= max_computed)
        return memo[n];

    /* Compute missing terms up to n (assumes n ≤ 10000). */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = (seq_t)1884   * memo[i - 4]
                + (seq_t)28048  * memo[i - 6]
                + (seq_t)134800 * memo[i - 8]
                + (seq_t)241600 * memo[i - 10]
                + (seq_t)86848  * memo[i - 12]
                - (seq_t)80384  * memo[i - 14]
                + (seq_t)25600  * memo[i - 16];
        memo[i] = v;
    }
    max_computed = n;
    return memo[n];
}
