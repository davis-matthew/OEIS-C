/*
 * A238894  Irregular triangle of the number of times that sums +- 3 +- 5 +- 7
 *          +- 11 +-...+- prime(2n+1) equal an even number in the range -d to
 *          d, where d = 3 + 5 + 7 + 11 +...+ prime(2n+1).
 * https://oeis.org/A238894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1070, u16 n <= 4982, u32 n <= 9025, u64 n <= 9025,
 *            u128 n <= 9025, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained from the “irregular triangle” described
 * in OEIS A238894.  For a given row k (k ≥ 1) the primes used are
 *
 *     prime(2), prime(3), …, prime(2k+1)
 *
 * (i.e. all odd primes up to the (2k+1)-st prime).  Let
 *
 *     d = sum of these primes.
 *
 * All possible sums ∑ ±p_i produce values in the range [–d, d] in
 * steps of 2.  For each even value x we count how many choices of
 * signs give that sum; the counts form the row of length d + 1.
 * The sequence a(n) is the concatenation of these rows, starting
 * with n = 1.
 *
 * The implementation below builds rows on demand up to the
 * largest row needed to answer a query.  Each row is generated
 * by dynamic programming over the prime list – a standard
 * 0‑1 knapsack count.  The primes up to the 45‑th one (229) are
 * sufficient for all rows that contribute terms up to index 10 000.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Prime list up to index 45 (prime(45)=229). */
static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229
};

/* Build one triangle row.  row_no is 1‑based.  The array contains
 * counts for sums from –d to +d in steps of 2 – index 0 corresponds
 * to –d, index 1 to –d+2, …, index d to +d.  The caller must free
 * the returned array. */
static seq_t *build_row(int row_no, size_t *out_len)
{
    int num_primes = 2 * row_no;          /* primes from prime(2) to prime(2k+1) */
    const uint32_t *p_start = primes + 1; /* skip the even prime 2 */
    seq_t d = 0;
    for (int i = 0; i < num_primes; ++i) d += p_start[i];

    size_t len = (size_t)d + 1;           /* number of even sums, 0 … d */
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) return NULL;
    dp[0] = 1;

    for (int i = 0; i < num_primes; ++i) {
        uint32_t p = p_start[i];
        /* update dp in reverse order to avoid reuse of new values */
        for (size_t s = len; s > p; --s) {
            dp[s - 1] += dp[s - 1 - p];
        }
    }

    *out_len = len;
    return dp;
}

/* Global state: cached rows and the concatenated sequence. */
seq_t a(idx_t n)
{
    /* 1‑based indexing for the sequence. */
    static bool initialized = false;
    static seq_t *all_vals = NULL;
    static size_t all_cap  = 0;
    static size_t all_len  = 0;

    static size_t *row_start = NULL;  /* start index of each row in all_vals (0‑based) */
    static size_t *row_len   = NULL;  /* length of each row */
    static size_t rows_cap   = 0;
    static size_t num_rows   = 0;

    size_t target = (size_t)(n - 1);   /* zero‑based index */

    /* First initialisation */
    if (!initialized) {
        initialized = true;
        all_cap = 1024;
        all_vals = (seq_t *)malloc(all_cap * sizeof(seq_t));

        rows_cap = 64;
        row_start = (size_t *)malloc(rows_cap * sizeof(size_t));
        row_len   = (size_t *)malloc(rows_cap * sizeof(size_t));
    }

    /* Build new rows until the requested index is covered. */
    while (all_len <= target) {
        int next_row = (int)(num_rows + 1);

        /* Ensure room for metadata of the new row. */
        if (num_rows >= rows_cap) {
            rows_cap = rows_cap ? rows_cap * 2 : 32;
            row_start = (size_t *)realloc(row_start, rows_cap * sizeof(size_t));
            row_len   = (size_t *)realloc(row_len,   rows_cap * sizeof(size_t));
        }

        size_t len;
        seq_t *row = build_row(next_row, &len);
        if (!row) return 0;  /* allocation failure – return 0 as a safe fallback */

        row_start[num_rows] = all_len;
        row_len[num_rows]   = len;
        ++num_rows;

        /* Make room for new values if necessary. */
        if (all_len + len > all_cap) {
            all_cap = (all_cap + len) * 2;
            all_vals = (seq_t *)realloc(all_vals, all_cap * sizeof(seq_t));
        }

        memcpy(all_vals + all_len, row, len * sizeof(seq_t));
        free(row);
        all_len += len;
    }

    /* Identify the row that contains the target index. */
    size_t r = 0;
    while (r < num_rows && row_start[r] + row_len[r] <= target) ++r;

    size_t offset = target - row_start[r];
    return all_vals[row_start[r] + offset];
}
