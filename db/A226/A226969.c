/*
 * A226969  Numbers whose base-4 sum of digits is 4.
 * https://oeis.org/A226969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 322, u32 n <= 3860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th positive integer whose base‑4 digit sum
   equals 4.  The calculation is performed by combinatorial enumeration
   (digit DP) and works for any seq_t / idx_t supplied by the build
   system.  The routine is independent of the value of n; calls may be
   made in any order. */
#include <stddef.h>

/* compile‑time constant: maximum number of base‑4 digits required for
   n up to 10 000 (worst case when every 4‑digit sum is 4).  The value
   is chosen so that the algorithm covers all requested indices. */
enum { MAX_LEN = 64 };

/* cnt[k][s] = number of length‑k base‑4 digit strings with digit sum s
   (digits 0‑3, leading zeros allowed).  k ranges from 0 to MAX_LEN,
   s ranges from 0 to 4. */
static idx_t cnt[MAX_LEN + 1][5];
static int initialized = 0;

/* Pre‑compute the combinatorial table once. */
static void
init_counts(void)
{
    cnt[0][0] = 1;
    for (int s = 1; s <= 4; ++s)
        cnt[0][s] = 0;

    for (int k = 1; k <= MAX_LEN; ++k) {
        for (int s = 0; s <= 4; ++s) {
            idx_t sum = 0;
            for (int d = 0; d <= 3; ++d)
                if (s >= d)
                    sum += cnt[k - 1][s - d];
            cnt[k][s] = sum;
        }
    }
    initialized = 1;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n = 0 */

    if (!initialized)
        init_counts();

    /* Find the length of the number that contains the nth element. */
    idx_t cumulative = 0;
    int   len = 0;
    for (len = 1; len <= MAX_LEN; ++len) {
        /* Count numbers of this length with digit sum 4 and
           leading digit ≥ 1. */
        idx_t total =
            cnt[len - 1][3] +   /* leading digit 1, remaining sum 3 */
            cnt[len - 1][2] +   /* leading digit 2, remaining sum 2 */
            cnt[len - 1][1];    /* leading digit 3, remaining sum 1 */
        if (cumulative + total >= n)
            break;
        cumulative += total;
    }
    if (len > MAX_LEN)
        return (seq_t)0;          /* n too large for pre‑computed range */

    idx_t rank = n - cumulative;   /* 1‑based rank within this length */
    int   sum_remaining = 4;
    seq_t value = 0;

    /* Construct the number from most significant digit to least. */
    for (int pos = len - 1; pos >= 0; --pos) {
        int start_d = (pos == len - 1) ? 1 : 0;
        for (int d = start_d; d <= 3; ++d) {
            if (d > sum_remaining)
                break;
            idx_t sub = cnt[pos][sum_remaining - d];
            if (rank > sub) {
                rank -= sub;
            } else {
                value = value * (seq_t)4 + (seq_t)d;
                sum_remaining -= d;
                break;
            }
        }
    }
    return value;
}
