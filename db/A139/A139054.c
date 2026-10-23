/*
 * A139054  Array read by rows: row n lists the first 4 primes with n digits.
 * https://oeis.org/A139054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A139054: the first four primes with exactly d decimal digits
   for each digit length d, arranged row-wise.  The function generates the
   required primes on demand and caches them.  It works for any seq_t
   supported by the build, including arbitrary-precision integers. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

/* Each entry stores the four primes for a given digit length. */
typedef struct {
    seq_t primes[4];
    bool  computed;
} row_t;

static row_t *rows   = NULL;   /* dynamic array indexed by digit length  - 1 */
static size_t rows_cap = 0;    /* allocated capacity (number of rows) */

/* Ensure that rows[0..max_d-1] exist.  Grow the table if necessary. */
static void ensure_rows(idx_t max_d)
{
    if (max_d + (idx_t)1 <= (idx_t)rows_cap)
        return;

    size_t new_cap = (size_t)(max_d + (idx_t)1);
    row_t *tmp = (row_t *)realloc(rows, new_cap * sizeof(row_t));
    if (!tmp) /* allocation failed – leave state unchanged */
        return;

    /* Zero‑initialize any newly added rows */
    size_t old_bytes = rows_cap * sizeof(row_t);
    size_t new_bytes = new_cap * sizeof(row_t);
    memset((char *)tmp + old_bytes, 0, new_bytes - old_bytes);

    rows = tmp;
    rows_cap = new_cap;
}

/* Generate the first four primes with exactly 'digits' decimal digits
   and store them in rows[digits-1]. */
static void compute_primes_for_digit(idx_t digits)
{
    row_t *r = &rows[digits - 1];

    seq_t candidate = pow10(digits - 1);
    if (candidate <= (seq_t)2)
        candidate = (seq_t)2;

    idx_t found = 0;
    seq_t one   = (seq_t)1;
    while (found < 4) {
        if (is_prime_big(candidate)) {
            r->primes[found] = candidate;
            ++found;
        }
        candidate = candidate + one;
    }
    r->computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* Determine the digit length (row) and column position. */
    idx_t digit_len = ((n - 1) / 4) + 1;   /* row number (digit length)   */
    idx_t pos_in_row = (n - 1) % 4;        /* 0‑based position in the row */

    /* Ensure the rows array is large enough. */
    ensure_rows(digit_len);
    if (rows == NULL)          /* allocation failed */
        return (seq_t)0;

    row_t *row = &rows[digit_len - 1];
    if (!row->computed)
        compute_primes_for_digit(digit_len);

    return row->primes[pos_in_row];
}
