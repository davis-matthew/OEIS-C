/*
 * A050753  Sixth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th sixth power with no consecutive equal digits
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 313, u128 n <= 1000,
 *            big any n
 * Helpers:   no_consecutive_equal_digits, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* static state */
static seq_t *sixth_vals = NULL;
static size_t sixth_len = 0;
static size_t sixth_cap = 0;
static idx_t next_base = 0;

static void grow_buffer(void)
{
    size_t new_cap = sixth_cap ? sixth_cap * 2 : 16;
    seq_t *new_buf = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < sixth_len; ++i) {
        new_buf[i] = sixth_vals[i];
    }
    if (sixth_vals) {
        seq_free_array(sixth_vals);
    }
    sixth_vals = new_buf;
    sixth_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* generate until we have n+1 terms */
    while (sixth_len <= n) {
        seq_t base = (seq_t)next_base;
        seq_t sq  = base * base;      /* base^2 */
        seq_t q   = sq * sq;          /* base^4 */
        seq_t val = q * sq;           /* base^6 */

        if (no_consecutive_equal_digits(val)) {
            if (sixth_len == sixth_cap) {
                grow_buffer();
            }
            sixth_vals[sixth_len++] = val;
        }
        ++next_base;
    }
    return sixth_vals[n];
}
