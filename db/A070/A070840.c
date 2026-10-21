/*
 * A070840  Repdigits (A010785) ordered by sum of digits (A007953).
 * https://oeis.org/A070840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = repdigits ordered by digit sum with a(1) = 0. For a given
 *            digit sum s, all repdigits d repeated l times where d * l = s
 *            are listed in increasing numeric order
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *repunit = NULL;   /* repunit[k] = 111…1 (k digits), index 0 unused */
static size_t repunit_cap = 0;

/* Ensure repunit[k] exists; extend array as necessary. */
static void ensure_repunit(size_t k)
{
    if (k <= repunit_cap)
        return;

    size_t new_cap = k;
    repunit = (seq_t *)realloc(repunit, (new_cap + 1) * sizeof(seq_t));
    if (!repunit)
        abort();

    if (repunit_cap == 0) {
        repunit[0] = 0;          /* unused element for convenient indexing */
        repunit[1] = 1;
        repunit_cap = 1;
    }

    for (size_t i = repunit_cap + 1; i <= new_cap; ++i) {
        repunit[i] = repunit[i - 1] * 10 + 1;
    }

    repunit_cap = new_cap;
}

/* Compute the n‑th term of OEIS sequence A070840. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* a(1) = 0 */

    seq_t buffer[10];             /* maximum 9 divisors per sum */
    idx_t count = 1;              /* a(1) already counted */

    for (idx_t sum = 1; count < n; ++sum) {
        size_t vcnt = 0;

        for (int d = 1; d <= 9; ++d) {
            if (sum % d == 0) {               /* integer divisor */
                idx_t k = sum / d;            /* length of repdigit */
                ensure_repunit((size_t)k);
                buffer[vcnt++] = (seq_t)d * repunit[(size_t)k];
            }
        }

        /* Sort the generated values for this digit sum (simple insertion). */
        for (size_t i = 1; i < vcnt; ++i) {
            seq_t key = buffer[i];
            size_t j = i;
            while (j > 0 && buffer[j - 1] > key) {
                buffer[j] = buffer[j - 1];
                --j;
            }
            buffer[j] = key;
        }

        for (size_t i = 0; i < vcnt; ++i) {
            ++count;
            if (count == n)
                return buffer[i];
        }
    }

    return (seq_t)0;   /* should not reach */
}
