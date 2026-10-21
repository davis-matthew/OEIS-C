/*
 * A207812  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=42, a(2)=1764. For n≥3 the sequence is obtained by dynamic
 *            programming on 7‑bit column patterns that avoid vertical triples
 *            010 and 101. Let M be the number of admissible patterns (M=42).
 *            Let good[i][j] be 1 if (pattern[i]|pattern[j])==0x7F. Define
 *            f₃[j]=∑i good[i][j] and for k≥3 f_{k+1}[j]=...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*----- static data, initialised once ---------------------------------*/
    static bool init_done = false;
    static size_t M = 0;               /* number of column patterns     */
    static uint8_t *patterns = NULL;   /* list of admissible patterns   */
    static unsigned char *good   = NULL;/* M×M table of (i|j)==ALL?     */
    static seq_t *f_cur = NULL;        /* f values for current length   */
    static seq_t *f_nxt = NULL;        /* temporary buffer for next step*/
    static seq_t *a_seq = NULL;        /* memoised a(n) values          */
    static size_t a_cap = 0;           /* capacity of a_seq             */
    static idx_t  a_len = 0;           /* largest n already computed    */
    static idx_t  cur_len = 0;         /* length that f_cur holds       */

    if (!init_done) {
        /* Build all admissible 7‑bit column patterns (avoid 010,101) */
        patterns = (uint8_t*)malloc(128);
        for (uint8_t p = 0; p < 128; ++p) {
            bool ok = true;
            for (int pos = 0; pos < 5; ++pos) {
                uint8_t tri = (p >> pos) & 0x7;
                if (tri == 0b010 || tri == 0b101) { ok = false; break; }
            }
            if (ok) patterns[M++] = p;
        }

        /* Pre‑compute good matrix */
        good = (unsigned char*)calloc(M * M, sizeof(unsigned char));
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < M; ++j)
                good[i * M + j] = ((patterns[i] | patterns[j]) == 0x7F);

        /* Allocate working buffers for f values */
        f_cur = (seq_t*)calloc(M, sizeof(seq_t));
        f_nxt = (seq_t*)calloc(M, sizeof(seq_t));

        /* Allocate array for memoised a(n) values */
        a_cap = 32;
        a_seq = (seq_t*)calloc(a_cap + 1, sizeof(seq_t)); /* 1‑based */

        init_done = true;
    }

    /* Guarantee capacity of a_seq */
    if ((size_t)n > a_cap) {
        size_t new_cap = a_cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_seq, (new_cap + 1) * sizeof(seq_t));
        if (tmp) { a_seq = tmp; a_cap = new_cap; }
    }

    /* Compute terms up to n if necessary */
    while (a_len < n) {
        if (a_len == 0) {                         /* a(1) */
            a_seq[1] = (seq_t)M;
            a_len = 1; cur_len = 1;
            continue;
        }
        if (a_len == 1) {                         /* a(2) */
            a_seq[2] = (seq_t)M * (seq_t)M;
            a_len = 2; cur_len = 2;
            continue;
        }
        if (a_len == 2) {                         /* compute f_cur for length 3 */
            for (size_t j = 0; j < M; ++j) {
                seq_t s = 0;
                for (size_t i = 0; i < M; ++i)
                    if (good[i * M + j]) s += (seq_t)1;
                f_cur[j] = s;
            }
            seq_t sum = 0;
            for (size_t j = 0; j < M; ++j) sum += f_cur[j];
            a_seq[3] = (seq_t)M * sum;
            a_len = 3; cur_len = 3;
            continue;
        }
        /* cur_len ≥ 3: extend to cur_len+1 */
        for (size_t k = 0; k < M; ++k) {
            seq_t s = 0;
            for (size_t j = 0; j < M; ++j)
                if (good[j * M + k]) s += f_cur[j];
            f_nxt[k] = s;
        }
        seq_t sum = 0;
        for (size_t k = 0; k < M; ++k) sum += f_nxt[k];
        a_seq[cur_len + 1] = (seq_t)M * sum;

        /* swap buffers and advance */
        seq_t *tmp = f_cur;  f_cur = f_nxt;  f_nxt = tmp;
        ++cur_len; ++a_len;
    }

    return a_seq[n];
}
