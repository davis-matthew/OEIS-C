/*
 * A175003  Triangle read by rows demonstrating Euler's pentagonal theorem for
 *          partition numbers.
 * https://oeis.org/A175003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 48, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175003 (Triangle read by rows demonstrating Euler's pentagonal theorem).
   Terms are linearized from the triangle:
   T(n,g) = s(g) * p(n - g)  where g runs through the generalized pentagonal numbers
   g = k(3k-1)/2 and k(3k+1)/2 (k = 1,2,…),
   s(g) = +1 for the first two pentagonal numbers of each k, and -1 for the next two,
   alternately.  The sequence a(n) is the concatenation of the rows (row 1 has
   length 1, row 2 has length 2, row 3 has length of pentagonal numbers ≤3, etc.).  */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *values      = NULL;   /* linearized triangle entries           */
static size_t values_cap  = 0;
static size_t values_len  = 0;

static seq_t *partitions  = NULL;   /* partition numbers p[0..]              */
static size_t part_cap    = 0;
static size_t part_len    = 0;       /* number of elements already stored     */

static idx_t next_row = 1;           /* next row to generate (1‑based)        */

/*--- Utility: grow a seq_t array ---------------------------------------*/
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/*--- Compute partition numbers p[0..n] -----------------------------------*/
static void ensure_partitions_to(idx_t n)
{
    if (part_len == 0) {
        /* initialize p(0) = 1 */
        grow_seq_t_array(&partitions, &part_cap, 1);
        partitions[0] = (seq_t)1;
        part_len = 1;
    }
    if (part_len > (size_t)n) return;   /* already computed */

    grow_seq_t_array(&partitions, &part_cap, (size_t)n + 1);
    for (idx_t m = part_len; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t sign = ((k % 2) == 1) ? (seq_t)1 : (seq_t)-1;
            sum += sign * partitions[(size_t)(m - g1)];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) sum += sign * partitions[(size_t)(m - g2)];
        }
        partitions[(size_t)m] = sum;
    }
    part_len = (size_t)n + 1;
}

/*--- Generate the next row of the triangle (row index n) ----------------*/
static void generate_row(idx_t n)
{
    ensure_partitions_to(n);

    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;
        bool any = false;
        if (g1 <= n) any = true;
        if (g2 <= n) any = true;
        if (!any) break;

        seq_t sign = ((k % 2) == 1) ? (seq_t)1 : (seq_t)-1;

        if (g1 <= n) {
            seq_t val = sign * partitions[(size_t)(n - g1)];
            grow_seq_t_array(&values, &values_cap, values_len + 1);
            values[values_len++] = val;
        }
        if (g2 <= n) {
            seq_t val = sign * partitions[(size_t)(n - g2)];
            grow_seq_t_array(&values, &values_cap, values_len + 1);
            values[values_len++] = val;
        }
    }
}

/*--- Ensure that we have generated terms up to the linear index 'need' ----*/
static void ensure_up_to(idx_t need)
{
    while (values_len < need) {
        generate_row(next_row);
        ++next_row;
    }
}

/*--- Public API ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */
    ensure_up_to(n);
    return values[(size_t)(n - 1)];
}
