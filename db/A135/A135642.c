/*
 * A135642  Concave numbers.
 * https://oeis.org/A135642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number consisting of decimal digits whose second
 *            difference is at most zero and at least one second difference is
 *            negative (strictly concave digits). The numbers are listed in
 *            increasing numeric order
 * Range:     u8 n <= 83, u16 n <= 4037, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum length needed for n up to 10 000 is comfortably below 25 digits */
#define MAX_LEN 25
/* diff values are in the range [-9,9], indexed by +9 */
#define DIFF_RANGE 19

/* forward declarations of helper functions */
static void build_dp(idx_t L, uint64_t dp[MAX_LEN][10][DIFF_RANGE][2]);
static idx_t total_length(idx_t L, uint64_t dp[MAX_LEN][10][DIFF_RANGE][2]);

seq_t a(idx_t n) {
    /* ---------- find the length of the desired number ---------- */
    idx_t L;                  /* length of the number to construct      */
    idx_t cumulative = 0;      /* count of numbers with shorter lengths */
    uint64_t dp[MAX_LEN][10][DIFF_RANGE][2];
    for (L = 3; ; ++L) {       /* minimal length is 3 digits            */
        build_dp(L, dp);
        idx_t tot = total_length(L, dp);
        if (cumulative + tot >= n) break;
        cumulative += tot;
    }

    idx_t offset = n - cumulative - 1;  /* zero‑based offset within this length */

    /* ---------- construct the number digit by digit ---------- */
    unsigned char digits[MAX_LEN];      /* buffer to hold all digits */

    /* choose the first digit */
    for (int d0 = 1; d0 <= 9; ++d0) {
        idx_t cnt = 0;
        for (int d1 = 0; d1 < 10; ++d1) {
            int diff1 = d1 - d0;
            if (diff1 < -9 || diff1 > 9) continue;
            int di = diff1 + 9;
            cnt += dp[2][d1][di][0];
        }
        if (offset < cnt) {
            digits[0] = d0;
            /* choose the second digit */
            int pos = 1;           /* position of the last chosen digit */
            int last_digit = 0;
            int diff_prev = 0;
            bool flag = false;
            for (int d1 = 0; d1 < 10; ++d1) {
                int diff1 = d1 - d0;
                if (diff1 < -9 || diff1 > 9) continue;
                int di = diff1 + 9;
                idx_t cnt1 = dp[2][d1][di][0];
                if (offset < cnt1) {
                    digits[1] = d1;
                    pos = 2;           /* two digits are now chosen        */
                    last_digit = d1;
                    diff_prev = diff1;  /* last difference                     */
                    flag = false;       /* no strict decrease yet            */
                    break;
                } else {
                    offset -= cnt1;
                }
            }
            /* choose remaining digits */
            while (pos < L) {
                for (int nd = 0; nd < 10; ++nd) {
                    int diff_new = nd - last_digit;
                    if (diff_new <= diff_prev) {
                        int di_new = diff_new + 9;
                        bool new_flag = flag || (diff_new < diff_prev);
                        idx_t cntN = dp[pos + 1][nd][di_new][new_flag];
                        if (offset < cntN) {
                            digits[pos] = nd;
                            ++pos;
                            last_digit = nd;
                            diff_prev = diff_new;
                            flag = new_flag;
                            break;
                        } else {
                            offset -= cntN;
                        }
                    }
                }
            }
            /* ---------- finally convert digits to seq_t ---------- */
            seq_t result = 0;
            for (idx_t i = 0; i < L; ++i) {
                result = result * 10 + digits[i];
            }
            return result;
        } else {
            offset -= cnt;
        }
    }
    /* the function should never reach here for valid n */
    return 0;
}

/* ----- helper functions ----- */

/* Build DP table for a given length L.
   dp[pos][ld][diff+9][flag] contains the number of ways to complete the
   sequence from the state (pos digits processed, last digit ld,
   previous difference diff, strict flag flag) to reach exactly L digits
   while maintaining concave property and finally flag==true. */
static void build_dp(idx_t L, uint64_t dp[MAX_LEN][10][DIFF_RANGE][2]) {
    /* initialize final position L: only flag==true yields 1 */
    for (int ld = 0; ld < 10; ++ld) {
        for (int di = 0; di < DIFF_RANGE; ++di) {
            dp[(int)L][ld][di][0] = 0;
            dp[(int)L][ld][di][1] = 1;
        }
    }

    /* fill positions from L‑1 down to 2 */
    for (int pos = (int)L - 1; pos >= 2; --pos) {
        for (int ld = 0; ld < 10; ++ld) {
            for (int di = 0; di < DIFF_RANGE; ++di) {
                int diff_prev = di - 9;
                for (int flag = 0; flag < 2; ++flag) {
                    uint64_t total = 0;
                    for (int nd = 0; nd < 10; ++nd) {
                        int diff_new = nd - ld;
                        if (diff_new <= diff_prev) {
                            int di_new = diff_new + 9;
                            int new_flag = flag || (diff_new < diff_prev);
                            total += dp[pos + 1][nd][di_new][new_flag];
                        }
                    }
                    dp[pos][ld][di][flag] = total;
                }
            }
        }
    }
}

/* Count all concave numbers of exact length L. */
static idx_t total_length(idx_t L, uint64_t dp[MAX_LEN][10][DIFF_RANGE][2]) {
    idx_t total = 0;
    for (int d0 = 1; d0 <= 9; ++d0) {
        for (int d1 = 0; d1 < 10; ++d1) {
            int diff1 = d1 - d0;
            if (diff1 < -9 || diff1 > 9) continue;
            int di = diff1 + 9;
            total += dp[2][d1][di][0];
        }
    }
    return total;
}
