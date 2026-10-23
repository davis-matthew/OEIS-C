/*
 * A024511  Positions of odd numbers in A004431 (sums of 2 distinct nonzero
 *          squares).
 * https://oeis.org/A024511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024511: Positions of odd numbers in A004431.
   The odd sums are all numbers of the form a^2+b^2 with 1≤a<b.
   An odd sum occurs iff exactly one of a and b is odd.
   We generate the sums up to a chosen limit, deduplicate by using a
   boolean visited array, collect the positions (1‑based rank) of the odd
   sums in the sorted list of all sums, and return the nth such position. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global containers – created on demand and grown when necessary. */
static seq_t *odd_positions = NULL;   /* positions of odd sums, 1‑based */
static size_t odd_positions_cap = 0;
static size_t odd_positions_len = 0;

static uint8_t *visited = NULL;       /* boolean map of sums up to current_limit */
static idx_t current_limit = 0;

/* Rebuild all data up to a new limit.  Previous arrays are freed. */
static void build_up_to(idx_t limit)
{
    /* free previous data */
    if (visited) free(visited);
    if (odd_positions) free(odd_positions);
    visited = NULL;
    odd_positions = NULL;
    odd_positions_cap = 0;
    odd_positions_len = 0;

    /* allocate visited flag array */
    visited = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!visited) exit(EXIT_FAILURE);

    /* generate all distinct pairs a<b where a^2+b^2 <= limit */
    for (idx_t a = 1; a * a <= limit; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = a + 1; a2 + b * b <= limit; ++b) {
            idx_t sum = a2 + b * b;
            visited[sum] = 1;
        }
    }

    /* collect odd positions */
    odd_positions_cap = 1024;
    odd_positions = (seq_t*)malloc(odd_positions_cap * sizeof(seq_t));
    if (!odd_positions) exit(EXIT_FAILURE);
    odd_positions_len = 0;

    idx_t pos = 0;   /* position in the sorted list, 1‑based */
    for (idx_t sum = 1; sum <= limit; ++sum) {
        if (visited[sum]) {
            ++pos;
            if (sum & 1) {   /* odd sum */
                if (odd_positions_len == odd_positions_cap) {
                    odd_positions_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(odd_positions,
                                                odd_positions_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    odd_positions = tmp;
                }
                odd_positions[odd_positions_len++] = (seq_t)pos;
            }
        }
    }

    current_limit = limit;
}

/* Main sequence function required by the build */
seq_t a(idx_t n)
{
    /* initial limit – can be tuned for typical n */
    if (current_limit == 0) {
        build_up_to(2000000);   /* start with 2 million */
    }

    /* enlarge data until we have enough odd positions */
    while (n > odd_positions_len) {
        /* double the limit each time */
        idx_t new_limit = current_limit * 2;
        /* guard against overflow */
        if (new_limit <= current_limit) new_limit = current_limit + 1000000;
        build_up_to(new_limit);
    }

    return odd_positions[n - 1];
}
