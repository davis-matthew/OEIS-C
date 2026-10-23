/*
 * A106567  a(n) = 5*a(n-1) + 4*a(n-2), with a(0) = 4, a(1) = 4.
 * https://oeis.org/A106567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2), a(0)=0, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_dp      = NULL; /* dynamic array of computed terms */
static size_t  seq_cap   = 0;     /* allocated capacity */
static size_t  seq_len   = 0;     /* number of terms stored (== seq_cap or less) */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases */
    if (n == 0) return 0;
    if (n == 1) return 4;

    /* Initialise storage on first use */
    if (seq_dp == NULL) {
        seq_cap = 2;
        seq_dp  = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_dp == NULL) return 0; /* allocation failed – return zero */
        seq_dp[0] = 0;
        seq_dp[1] = 4;
        seq_len  = 2;
    }

    /* Grow the table if necessary and compute missing terms */
    if ((size_t)n >= seq_len) {
        /* Expand capacity as needed */
        while (seq_cap <= (size_t)n) {
            size_t new_cap = seq_cap * 2;
            if (new_cap <= seq_cap) new_cap = seq_cap + 10; /* safety */
            seq_t *tmp = (seq_t *)realloc(seq_dp, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failed */
            seq_dp = tmp;
            seq_cap = new_cap;
        }

        /* Compute terms from seq_len up to n */
        while (seq_len <= (size_t)n) {
            seq_t a_nm2 = seq_dp[seq_len - 2];
            seq_t a_nm1 = seq_dp[seq_len - 1];
            seq_t val = 5 * a_nm1 + 4 * a_nm2;
            seq_dp[seq_len] = val;
            ++seq_len;
        }
    }

    return seq_dp[(size_t)n];
}
