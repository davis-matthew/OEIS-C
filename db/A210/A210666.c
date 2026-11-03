/*
 * A210666  Numbers with at least three digits in which all digits but one are
 *          the same.
 * https://oeis.org/A210666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 802, u32 n <= 3678, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9949 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A210666.
   Each term is the smallest integer having at least 3 digits,
   where exactly one digit is distinct and all others are equal.
   The function enumerates all k‑digit numbers with this property,
   sorts them in ascending order, and returns the n‑th term. */

#include <stdlib.h>
#include <string.h>

/* Helper: compare two null‑terminated digit strings lexicographically. */
static int compare_str(const void *a, const void *b) {
    const char * const *sa = (const char * const *)a;
    const char * const *sb = (const char * const *)b;
    return strcmp(*sa, *sb);
}

/* Helper: generate all k‑digit numbers with exactly one digit distinct.
   The result is an array of pointers to null‑terminated strings.
   *out_vec points to the array; *out_cnt receives its length. */
static void generate_block(int k, char ***out_vec, size_t *out_cnt) {
    /* Upper bound on number of combinations: 81 * k (10 choices for r,
       9 for d2, k positions). */
    size_t max = 81u * k;
    char **vec = (char **)malloc(max * sizeof(char *));
    size_t cnt = 0;

    for (int r = 0; r < 10; ++r) {                 /* repeated digit */
        for (int d2 = 0; d2 < 10; ++d2) {           /* distinct digit */
            if (d2 == r) continue;
            for (int p = 0; p < k; ++p) {           /* position of d2 */
                /* Leading zero check */
                if (p == 0) {
                    if (d2 == 0) continue;          /* leading zero */
                } else {
                    if (r == 0) continue;           /* leading zero */
                }
                char *buf = (char *)malloc((k + 1) * sizeof(char));
                for (int pos = 0; pos < k; ++pos) {
                    buf[pos] = (pos == p) ? (char)(d2 + '0') : (char)(r + '0');
                }
                buf[k] = '\0';
                vec[cnt++] = buf;
            }
        }
    }
    *out_vec = vec;
    *out_cnt = cnt;
}

/* Helper: parse a decimal string into seq_t.  Numbers that fit into
   seq_t are parsed exactly; larger numbers will overflow, but those
   are never required for correct results. */
static seq_t parse_str(const char *s) {
    seq_t val = 0;
    while (*s) {
        val = val * 10 + (seq_t)(*s - '0');
        ++s;
    }
    return val;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t cumulative = 0;
    int k = 3;

    while (1) {
        char **vec = NULL;
        size_t cnt = 0;
        generate_block(k, &vec, &cnt);

        if (cumulative + (idx_t)cnt >= n) {
            /* Desired block found: sort and pick the element. */
            qsort(vec, cnt, sizeof(char *), compare_str);
            size_t idx = (size_t)(n - cumulative - 1);
            seq_t result = parse_str(vec[idx]);

            /* Free memory before returning. */
            for (size_t i = 0; i < cnt; ++i) free(vec[i]);
            free(vec);
            return result;
        }

        /* Not in this block: free and continue to next k. */
        for (size_t i = 0; i < cnt; ++i) free(vec[i]);
        free(vec);
        cumulative += (idx_t)cnt;
        ++k;
    }
}
