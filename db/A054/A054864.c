/*
 * A054864  Numbers not divisible by any of their digits when written in base
 *          8.
 * https://oeis.org/A054864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: linear scan with memoisation (numbers whose base‑8 digits
   do not all divide the number).  a(1)=19, a(2)=21, … as in OEIS A054864. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for memoisation – 1‑based index, but the array itself is
   0‑based, so values[0] holds a(1). */
static uint64_t *values = NULL;       /* array of uint64_t values */
static size_t   values_len = 0;       /* number of stored terms   */
static size_t   values_cap = 0;       /* allocated capacity for values[] */

/* next integer to test for validity */
static uint64_t next_candidate = 1;

/* test whether n is not divisible by any of its non‑zero base‑8 digits */
static bool candidate_ok(uint64_t n)
{
    uint64_t rem = n;
    while (rem) {
        int d = rem & 7;             /* digit = rem % 8   */
        if (d > 0 && (n % (uint64_t)d) == 0) return false;
        rem >>= 3;                    /* remove last digit */
    }
    return true;
}

/* append a new term to the memoisation array */
static void store_value(uint64_t val)
{
    if (values_len == values_cap) {
        size_t new_cap = values_cap ? values_cap << 1 : 8;
        uint64_t *tmp = (uint64_t *)realloc(values, new_cap * sizeof(uint64_t));
        values = tmp;
        values_cap = new_cap;
    }
    values[values_len++] = val;
}

/* main entry: return a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n == 0, guard */

    while (values_len < (size_t)n) {
        while (!candidate_ok(next_candidate))
            ++next_candidate;
        store_value(next_candidate);
        ++next_candidate;
    }
    return (seq_t)values[n - 1];
}
