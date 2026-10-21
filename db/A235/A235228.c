/*
 * A235228  Numbers whose sum of digits is 18.
 * https://oeis.org/A235228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th integer whose decimal digit sum equals 18
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define SUM 18
#define MAXLEN 200   /* enough for all practical queries up to n≈10⁶ */

static uint64_t ways[MAXLEN + 1][SUM + 1];
static bool initialized = false;

static void init_dp(void) {
    if (initialized) return;
    /* Base cases */
    for (int s = 0; s <= SUM; ++s)
        ways[0][s] = (s == 0) ? 1 : 0;
    for (int l = 1; l <= MAXLEN; ++l) {
        for (int s = 0; s <= SUM; ++s) {
            uint64_t total = 0;
            for (int d = 0; d <= 9 && d <= s; ++d) {
                total += ways[l - 1][s - d];
            }
            ways[l][s] = total;
        }
    }
    initialized = true;
}

/* Count of length `len` numbers with digit sum SUM and first digit >=1 */
static uint64_t count_len(int len) {
    uint64_t cnt = 0;
    for (int d = 1; d <= 9 && d <= SUM; ++d) {
        cnt += ways[len - 1][SUM - d];
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, return dummy */
    init_dp();

    /* Find length such that cumulative count ≥ n */
    uint64_t cumulative = 0;
    int len = 1;
    uint64_t this_len = 0;
    while (1) {
        if (len > MAXLEN) break;  /* safety, should never happen for tested n */
        this_len = count_len(len);
        if (cumulative + this_len >= n)
            break;
        cumulative += this_len;
        ++len;
    }

    /* Position within numbers of this length (1‑based) */
    uint64_t offset = n - cumulative;  /* 1‑based index inside block */

    /* Construct the number digit by digit */
    seq_t result = 0;
    int sum_rem = SUM;
    for (int pos = 0; pos < len; ++pos) {
        int min_digit = (pos == 0) ? 1 : 0;
        for (int d = min_digit; d <= 9 && d <= sum_rem; ++d) {
            uint64_t ways_here = ways[len - pos - 1][sum_rem - d];
            if (offset > ways_here) {
                offset -= ways_here;
            } else {
                /* Append digit d */
                result = result * (seq_t)10 + (seq_t)d;
                sum_rem -= d;
                break;
            }
        }
    }
    return result;
}
