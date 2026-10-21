/*
 * A209972  Number of binary words of length n avoiding the subword given by
 *          the binary expansion of k; square array A(n,k), n>=0, k>=0, read
 *          by antidiagonals.
 * https://oeis.org/A209972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 342, u32 n <= 1025, u64 n <= 3309,
 *            u128 n <= 9729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary words of length i avoiding the subword
 * given by the binary expansion of k, where (i,k) is derived from n
 * by antidiagonal enumeration.  This implements the definition of
 * OEIS A209972.  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---- find diagonal d with d(d+1)/2 <= n < (d+1)(d+2)/2 ---- */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t d = low;                              /* diagonal index          */
    idx_t tri_prev = d * (d + 1) / 2;            /* terms before this diag  */
    idx_t offset = n - tri_prev;                /* 0‑based offset          */
    idx_t i_len = offset;                       /* word length              */
    idx_t k = d - offset;                       /* subword number           */

    /* ---- convert k to binary pattern bits ---- */
    idx_t m;
    if (k == 0) {
        m = 1;                                  /* pattern "0"             */
    } else {
        idx_t tmp = k;
        m = 0;
        while (tmp > 0) {
            ++m;
            tmp >>= 1;
        }
    }
    int bits[64] = {0};
    for (int idx = 0; idx < (int)m; ++idx) {
        bits[idx] = (int)((k >> (m - 1 - idx)) & 1ULL);
    }

    /* ---- build KMP prefix function ---- */
    int pi[64] = {0};
    for (int idx = 1; idx < (int)m; ++idx) {
        int j = pi[idx - 1];
        while (j > 0 && bits[idx] != bits[j]) {
            j = pi[j - 1];
        }
        if (bits[idx] == bits[j]) {
            ++j;
        }
        pi[idx] = j;
    }

    /* ---- build transition table ---- */
    int trans[64][2];
    for (int j = 0; j < (int)m; ++j) {
        for (int b = 0; b < 2; ++b) {
            int jj = j;
            while (jj > 0 && bits[jj] != b) {
                jj = pi[jj - 1];
            }
            if (bits[jj] == b) {
                ++jj;
            }
            trans[j][b] = jj;   /* may equal m (forbidden state) */
        }
    }

    /* ---- dynamic programming over word length ---- */
    seq_t dp_cur[64] = {0};
    seq_t dp_next[64] = {0};
    dp_cur[0] = 1;                     /* start in state 0 */
    for (idx_t pos = 0; pos < i_len; ++pos) {
        for (int t = 0; t < (int)m; ++t) {
            dp_next[t] = 0;
        }
        for (int j = 0; j < (int)m; ++j) {
            seq_t val = dp_cur[j];
            if (val == 0) {
                continue;
            }
            for (int b = 0; b < 2; ++b) {
                int nxt = trans[j][b];
                if ((seq_t)nxt < (seq_t)m) {
                    dp_next[nxt] += val;
                }
            }
        }
        for (int t = 0; t < (int)m; ++t) {
            dp_cur[t] = dp_next[t];
        }
    }

    /* ---- sum all accepting states ---- */
    seq_t result = 0;
    for (int j = 0; j < (int)m; ++j) {
        result += dp_cur[j];
    }
    return result;
}
