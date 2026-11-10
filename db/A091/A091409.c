/*
 * A091409  a(n) is the smallest m such that A090822(m) = n.
 * https://oeis.org/A091409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = smallest m with A090822(m) = n, found by
 *            simulating Gijswijt's sequence. Only n <= 4 is feasible (a(5) is
 *            astronomically large)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A091409_LEN 400

static int A091409_g[A091409_LEN];
static bool A091409_ready = false;

static void A091409_init(void)
{
    int len = 0;
    A091409_g[len++] = 1;
    while (len < A091409_LEN) {
        int best = 0;
        if (len == 1) {
            best = 1;
        } else {
            for (int bl = 1; bl <= len; ++bl) {
                int start = len - bl;
                int count = 1;
                while (start >= bl) {
                    bool match = true;
                    for (int j = 0; j < bl; ++j) {
                        if (A091409_g[start + j] != A091409_g[start - bl + j]) {
                            match = false;
                            break;
                        }
                    }
                    if (!match) break;
                    ++count;
                    start -= bl;
                }
                if (count > best) best = count;
            }
        }
        A091409_g[len++] = best;
    }
    A091409_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 4) return (seq_t)0; /* a(5) is far beyond any feasible computation */
    if (!A091409_ready) A091409_init();
    for (int i = 0; i < A091409_LEN; ++i) {
        if ((idx_t)A091409_g[i] == n) return (seq_t)(i + 1);
    }
    return (seq_t)0;
}
