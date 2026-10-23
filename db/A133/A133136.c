/*
 * A133136  Powers of 3 written backwards and sorted.
 * https://oeis.org/A133136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the sorted list of the decimal digit reversals of the
 *            powers of 3, with the sequence offset 1. The function builds the
 *            list lazily and memoises it
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* In the OEIS test harness the type seq_t is defined elsewhere; it is an
   unsigned integer type or a big‑integer class that supports the normal
   arithmetic operators (+, -, *, /, %) and comparisons (<, >). */

/* Helper: reverse the decimal digits of a seq_t value. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Comparator for qsort on seq_t values. */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* the sequence is defined only for n>=1 */
        return 0;
    }

    /* Static data for memoisation. */
    static seq_t *values     = NULL;
    static size_t val_size   = 0;
    static size_t val_cap    = 0;
    static seq_t curr_power  = 1;   /* 3^0 */
    static idx_t  next_exp   = 0;    /* next exponent to generate */
    static int    sorted     = 1;    /* 1 = values currently sorted */

    /* We generate a few extra terms to be safe when the n‑th smallest
       lies inside a digit–length block that contains more than n terms.  */
    idx_t required = n + 5;          /* extra five terms are ample */

    /* Generate new terms until we have at least 'required' values. */
    while ((idx_t)val_size < required) {
        /* Grow the underlying array when needed. */
        if (val_size == val_cap) {
            size_t new_cap = (val_cap == 0 ? 16 : val_cap << 1);
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp) {   /* allocation failed – return zero (unlikely in tests) */
                return 0;
            }
            if (values) memcpy(tmp, values, val_size * sizeof(seq_t));
            free(values);
            values = tmp;
            val_cap = new_cap;
        }

        /* Store the reversed value of the current power of 3. */
        seq_t rev = reverse_digits(curr_power);
        values[val_size++] = rev;

        /* Prepare the next power of 3. */
        curr_power = curr_power * 3;   /* the operator is defined for seq_t */
        ++next_exp;

        sorted = 0;   /* new unsorted element added */
    }

    /* Ensure the array is sorted before indexing. */
    if (!sorted) {
        qsort(values, val_size, sizeof(seq_t), seq_compare);
        sorted = 1;
    }

    /* Return the (n-1)-th element (offset 1). */
    return values[n - 1];
}
