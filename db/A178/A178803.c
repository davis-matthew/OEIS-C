/*
 * A178803  Irregular triangle: T(n,k) is the factorial of the number of parts
 *          in the k-th partition of n listed in Abramowitz and Stegun order,
 *          read by rows, 1 <= k <= A000041(n).
 * https://oeis.org/A178803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 95, u32 n <= 371, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   factorial_of_parts, generate_partitions_by_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: a(n) = factorial of the number of parts of the partition whose
   linear index is n.  Partitions of each integer m are arranged by
   increasing number of parts (k) and, for equal k, by decreasing first
   part, which is the Abramowitz–Stegun order. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorial_of_parts.h"
#include "../utils/generate_partitions_by_k.h"

#define MAX_ROWS 2000   /* enough to cover indices up to ≈10 000 */

static idx_t  offsets[MAX_ROWS + 1];         /* start index of each row    */
static idx_t  part_counts[MAX_ROWS];         /* number of partitions in row*/
static uint8_t *parts_of_row[MAX_ROWS];      /* array of #parts for that row */
static int     computed_rows = -1;            /* highest row fully computed  */

/* Compute the row corresponding to the integer 'row' (i.e. partitions of
   'row') and update the global tables.  It stores the sorted array of
   part counts (by increasing number of parts) and the cumulative offset
   for the next row. */
static void compute_row(int row)
{
    size_t len = 0, cap = 0;
    uint8_t *parts = NULL;

    for (int k = 1; k <= row; ++k) {
        generate_partitions_by_k(row, row, k, 0, &parts, &len, &cap);
    }

    part_counts[row] = (idx_t)len;
    parts_of_row[row] = parts;
    offsets[row + 1] = offsets[row] + (idx_t)len;   /* start of next row */
}

/* Initialise row 0 (partition of 0: empty partition). */
static void init_row0(void)
{
    parts_of_row[0] = (uint8_t *)malloc(1);
    if (!parts_of_row[0]) exit(1);
    parts_of_row[0][0] = 0;          /* one partition with 0 parts */
    part_counts[0] = 1;
    offsets[0] = 0;
    offsets[1] = 1;                  /* first row occupies index 0 only */
    computed_rows = 0;
}

/* Main entry: returns the n‑th term of A178803. */
seq_t a(idx_t n)
{
    if (computed_rows < 0) {
        init_row0();                    /* ensure row 0 exists */
    }

    /* Extend pre‑computed rows until n is covered. */
    while (offsets[computed_rows + 1] <= n) {
        int next = computed_rows + 1;
        compute_row(next);
        computed_rows = next;
    }

    /* Determine which row contains index n. */
    idx_t row = computed_rows;
    idx_t index_in_row = n - offsets[row];

    uint8_t part_count = parts_of_row[row][index_in_row];
    return factorial_of_parts(part_count);
}
