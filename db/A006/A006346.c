/*
 * A006346  The Sally sequence: the length of repetition avoided in A006345.
 * https://oeis.org/A006346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   search/simulation. Build the Linus sequence A006345 (s(1)=1).
 *            Each new term is 1 or 2, whichever makes the longest square XX
 *            ending the sequence shorter. a(n) = |X| for the square created
 *            by the rejected choice (a(1) = 0). For each shift k, keep the
 *            length of the run of positions j with s(j) = s(...
 * Range:     u8 n <= 1139, u16 n <= 9541, u32 n <= 9541, u64 n <= 9541,
 *            u128 n <= 9541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *lin_s = NULL;
static uint32_t *lin_run = NULL;
static uint64_t *lin_res = NULL;
static idx_t lin_done = 0;
static idx_t lin_cap = 0;

static void lin_ensure(idx_t need) {
    idx_t newcap;
    if (need < lin_cap) return;
    newcap = lin_cap * 2;
    if (newcap < need + 1) newcap = need + 1;
    if (newcap < 64) newcap = 64;
    lin_s = (unsigned char *)realloc(lin_s, (size_t)newcap * sizeof(unsigned char));
    lin_run = (uint32_t *)realloc(lin_run, (size_t)newcap * sizeof(uint32_t));
    lin_res = (uint64_t *)realloc(lin_res, (size_t)newcap * sizeof(uint64_t));
    memset(lin_s + lin_cap, 0, (size_t)(newcap - lin_cap) * sizeof(unsigned char));
    memset(lin_run + lin_cap, 0, (size_t)(newcap - lin_cap) * sizeof(uint32_t));
    memset(lin_res + lin_cap, 0, (size_t)(newcap - lin_cap) * sizeof(uint64_t));
    lin_cap = newcap;
}

seq_t a(idx_t n) {
    idx_t m;
    if (n == 0) return 0;
    if (n > lin_done) {
        lin_ensure(n);
        for (m = lin_done + 1; m <= n; m++) {
            uint64_t lg[3];
            int c;
            unsigned char chosen;
            idx_t k;
            if (m == 1) {
                lin_s[1] = 1;
                lin_res[1] = 0;
                continue;
            }
            for (c = 1; c <= 2; c++) {
                lg[c] = 0;
                for (k = m / 2; k >= 1; k--) {
                    if (lin_s[m - k] == (unsigned char)c &&
                        (uint64_t)lin_run[k] + 1 >= (uint64_t)k) {
                        lg[c] = k;
                        break;
                    }
                }
            }
            if (lg[1] <= lg[2]) {
                chosen = 1;
                lin_res[m] = lg[2];
            } else {
                chosen = 2;
                lin_res[m] = lg[1];
            }
            lin_s[m] = chosen;
            for (k = 1; k < m; k++) {
                if (lin_s[m - k] == chosen) lin_run[k]++;
                else lin_run[k] = 0;
            }
        }
        lin_done = n;
    }
    return (seq_t)lin_res[n];
}
