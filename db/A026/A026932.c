/*
 * A026932  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into an even number of parts, each <= k.
 * https://oeis.org/A026932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth number obtained by reading the triangular array
 *            T read by rows, where T(m,k) = number of partitions of m into an
 *            even number of parts, each <= k
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static size_t current_max_c = 0;
static seq_t *dp_even = NULL;
static seq_t *dp_odd  = NULL;
static seq_t *table   = NULL;

/* Recompute the triangular table up to max_c (inclusive). */
static void precompute(size_t max_c)
{
    if (max_c <= current_max_c) return;

    free(dp_even);
    free(dp_odd);
    free(table);

    dp_even = (seq_t *)calloc(max_c + 1, sizeof(seq_t));
    dp_odd  = (seq_t *)calloc(max_c + 1, sizeof(seq_t));
    table   = (seq_t *)calloc((max_c + 1) * (max_c + 1), sizeof(seq_t));
    if (!dp_even || !dp_odd || !table) exit(1);

    dp_even[0] = 1; /* empty partition has 0 parts (even) */

    for (size_t k = 1; k <= max_c; ++k) {
        /* allow unlimited parts of size k */
        for (size_t s = k; s <= max_c; ++s) {
            dp_even[s] += dp_odd[s - k];  /* adding a part flips parity */
            dp_odd[s]  += dp_even[s - k];
        }
        /* store current dp_even values for this maximal part k */
        for (size_t s = 0; s <= max_c; ++s) {
            table[s * (max_c + 1) + k] = dp_even[s];
        }
    }
    current_max_c = max_c;
}

seq_t a(idx_t n)
{
    /* find the row r and column c that correspond to the index n
       (rows are 1‑based, columns are 1‑based). */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t c = n - (sum - r);

    precompute((size_t)r);

    size_t width = current_max_c + 1;
    return table[(size_t)r * width + (size_t)c];
}
