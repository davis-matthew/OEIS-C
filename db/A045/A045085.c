/*
 * A045085  Numbers whose base-4 representation contains exactly four 0's and
 *          four 3's.
 * https://oeis.org/A045085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly * four 0's
 *            and four 3's
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: binomial coefficient n choose k (small n) */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

/* Pre‑computed powers of 2 up to 31 (enough for all L needed). */
static seq_t pow2_table[32];
static int pow2_ready = 0;

/* Pre‑computed powers of 4 up to 31. */
static seq_t pow4_table[32];
static int pow4_ready = 0;

/* Count how many numbers have length L (most significant digit is not 0). */
static seq_t count_len(idx_t L)
{
    if (L < 8) return 0;
    if (L == 8) {
        /* Only leading digit 3 is possible. */
        return binom(7, 4) * binom(3, 3);          /* 35 */
    }
    /* For L >= 9 */
    seq_t cnt12 = binom(L-1, 4) * binom(L-5, 4) * pow2_table[L-9];
    seq_t cnt3  = binom(L-1, 4) * binom(L-5, 3) * pow2_table[L-8];
    return cnt12 * 2 + cnt3;
}

/* Count completions for the remaining positions. */
static seq_t count_completions(idx_t pos,
                               idx_t zeros, idx_t threes, idx_t ones)
{
    return binom(pos, zeros) * binom(pos - zeros, threes) * pow2_table[ones];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    /* Initialise tables once. */
    if (!pow2_ready) {
        pow2_table[0] = 1;
        for (int i = 1; i < 32; ++i) pow2_table[i] = pow2_table[i-1] << 1;
        pow2_ready = 1;
    }
    if (!pow4_ready) {
        pow4_table[0] = 1;
        for (int i = 1; i < 32; ++i) pow4_table[i] = pow4_table[i-1] * 4;
        pow4_ready = 1;
    }

    /* Find the length L that contains the n-th term. */
    idx_t cumulative = 0;
    idx_t L = 8;
    while (1) {
        seq_t cnt = count_len(L);
        if (cumulative + (idx_t)cnt >= n) break;
        cumulative += (idx_t)cnt;
        ++L;
    }

    /* Index within numbers of length L (0‑based). */
    idx_t rem = n - cumulative - 1;

    /* State variables. */
    idx_t zeros_rem   = 4;
    idx_t threes_rem  = 4;
    idx_t ones_rem    = L - 8;
    idx_t pos_left    = L;                 /* digits remaining (including first) */
    seq_t value = 0;

    /* Choose the most significant digit (cannot be 0). */
    for (int d = 1; d <= 3; ++d) {
        idx_t zr = zeros_rem, tr = threes_rem, orr = ones_rem;
        seq_t cnt;
        if (d == 1 || d == 2) {
            if (orr == 0) continue;
            --orr;
            cnt = count_completions(pos_left-1, zr, tr, orr);
        } else { /* d == 3 */
            if (tr == 0) continue;
            --tr;
            cnt = count_completions(pos_left-1, zr, tr, orr);
        }
        if ((idx_t)rem < (idx_t)cnt) {
            value = (seq_t)d * pow4_table[pos_left-1];
            zeros_rem = zr; threes_rem = tr; ones_rem = orr;
            pos_left -= 1;
            break;
        } else {
            rem -= (idx_t)cnt;
        }
    }

    /* Remaining digits from most significant to least. */
    while (pos_left > 0) {
        idx_t power = pos_left - 1;
        for (int d = 0; d <= 3; ++d) {
            if (d == 0 && zeros_rem == 0) continue;
            if (d == 3 && threes_rem == 0) continue;
            if (d == 1 || d == 2) { if (ones_rem == 0) continue; }
            idx_t zr = zeros_rem, tr = threes_rem, orr = ones_rem;
            if (d == 0) --zr;
            else if (d == 3) --tr;
            else --orr;
            seq_t cnt = count_completions(pos_left-1, zr, tr, orr);
            if ((idx_t)rem < (idx_t)cnt) {
                value += (seq_t)d * pow4_table[power];
                zeros_rem = zr; threes_rem = tr; ones_rem = orr;
                pos_left -= 1;
                break;
            } else {
                rem -= (idx_t)cnt;
            }
        }
    }

    return value;
}
