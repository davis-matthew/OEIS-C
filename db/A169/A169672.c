/*
 * A169672  Lexicographically earliest de Bruijn sequence for n = 5 and k = 2.
 * https://oeis.org/A169672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes lexicographically earliest de Bruijn sequence for n = 5, k = 2.
   The sequence repeats every 32 terms: 00000100011001010011101011011111. */

#include <stdbool.h>
#include <stddef.h>

static bool seq_initialized = false;
static seq_t debruijn_seq[32];

/* Recursive helper for the FKM algorithm. */
static void db(int t, int p, int n, int k, int *a, seq_t *seq, int *seq_len) {
    if (t > n) {
        if ((n % p) == 0) {
            for (int i = 1; i <= p; ++i) {
                seq[(*seq_len)++] = (seq_t)a[i];
            }
        }
    } else {
        a[t] = a[t - p];
        db(t + 1, p, n, k, a, seq, seq_len);
        for (int j = a[t - p] + 1; j < k; ++j) {
            a[t] = j;
            db(t + 1, t, n, k, a, seq, seq_len);
        }
    }
}

/* Lazily generate the de Bruijn sequence of order 5. */
static void generate_debruijn_seq(void) {
    if (seq_initialized) return;

    const int n = 5;
    const int k = 2;
    int a[n + 1];          /* 1‑based indexing, a[0] is used as a sentinel */
    a[0] = 0;              /* initialise the sentinel to zero */
    int seq_len = 0;
    db(1, 1, n, k, a, debruijn_seq, &seq_len);

    /* seq_len should be exactly 2^5 = 32 */
    seq_initialized = true;
}

seq_t a(idx_t n) {
    generate_debruijn_seq();
    return debruijn_seq[n & 31];
}
