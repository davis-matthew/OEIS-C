/*
 * A178361  Numbers with rounded up arithmetic mean of digits = 1.
 * https://oeis.org/A178361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 175, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th number whose rounded‑up arithmetic mean of decimal
   digits equals 1.  The number is the n‑th integer (n≥1) whose digit
   sum ≤ its number of digits.  The algorithm counts, for each
   candidate length d, how many such numbers exist, and then un‑
   ranks the desired one in numeric order.  The counts are pre‑computed
   with dynamic programming over digit sums (digits 0–9). */

#include <stdbool.h>

#define MAX_DIGITS 20          /* sufficient for n up to at least 10⁶   */
#define MAX_SUM    MAX_DIGITS  /* digits are 0…9 but the sum never
                                * exceeds the number of digits */

static seq_t dp[MAX_DIGITS + 1][MAX_SUM + 1];
static seq_t pref[MAX_DIGITS + 1][MAX_SUM + 1];
static bool tables_inited = false;

/* number of suffixes of length 'len' having total digit sum ≤ 'maxSum' */
static seq_t count_suffix(int len, int maxSum)
{
    if (maxSum < 0) return 0;
    if (maxSum > MAX_SUM) maxSum = MAX_SUM;
    return pref[len][maxSum];
}

/* build tables once */
static void init_tables(void)
{
    if (tables_inited) return;
    /* dp[l][s] – number of length‑l sequences with exact digit sum s */
    for (int s = 0; s <= MAX_SUM; ++s) dp[0][s] = (s == 0 ? 1 : 0);
    for (int l = 1; l <= MAX_DIGITS; ++l) {
        for (int s = 0; s <= MAX_SUM; ++s) {
            seq_t val = 0;
            int maxdigit = (s < 9 ? s : 9);      /* digit ≤9 and ≤s */
            for (int d = 0; d <= maxdigit; ++d) {
                val += dp[l - 1][s - d];
            }
            dp[l][s] = val;
        }
    }
    /* prefix sums for fast counting of “sum ≤ maxSum” */
    for (int l = 0; l <= MAX_DIGITS; ++l) {
        seq_t acc = 0;
        for (int s = 0; s <= MAX_SUM; ++s) {
            acc += dp[l][s];
            pref[l][s] = acc;
        }
    }
    tables_inited = true;
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    init_tables();

    idx_t target = n;
    idx_t cumulative = 0;
    int d = 0;
    idx_t len_count = 0;

    /* find the block length d containing the n-th element */
    for (;;) {
        ++d;
        len_count = 0;
        int max_first = (d < 9 ? d : 9);
        for (int first = 1; first <= max_first; ++first) {
            len_count += (idx_t)count_suffix(d - 1, d - first);
        }
        if (cumulative + len_count >= target) break;
        cumulative += len_count;
    }

    idx_t k = target - cumulative;   /* 1‑based rank within this block */
    seq_t result = 0;
    int remaining_sum = d;            /* digits may sum up to d */

    for (int pos = 1; pos <= d; ++pos) {
        int min_digit = (pos == 1 ? 1 : 0);
        int max_digit = (remaining_sum < 9 ? remaining_sum : 9);

        for (int digit = min_digit; digit <= max_digit; ++digit) {
            int rem_len = d - pos;
            int rem_sum = remaining_sum - digit;
            seq_t ways = count_suffix(rem_len, rem_sum);
            idx_t ways_idx = (idx_t)ways;

            if (k > ways_idx) {
                k -= ways_idx;
            } else {
                result = result * (seq_t)10 + (seq_t)digit;
                remaining_sum = rem_sum;
                break;
            }
        }
    }
    return result;
}
