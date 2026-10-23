/*
 * A162823  Positive numbers k such that 42*k/(42-k) are integers.
 * https://oeis.org/A162823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 42 ± d for positive divisor d of 1764, plus the special
 *            k=42
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers (C11/C++17). */
#include <stdbool.h>
#include <stdlib.h>

/* Comparison function for qsort. */
static int cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Cached sequence values. */
static bool initialized = false;
static seq_t vals[50];
static idx_t sz = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Enumerate positive divisors of 1764. */
        seq_t divisors[100];
        idx_t dcnt = 0;
        for (idx_t i = 1; i * i <= 1764; ++i) {
            if (1764 % i == 0) {
                divisors[dcnt++] = (seq_t)i;
                idx_t j = 1764 / i;
                if (j != i)
                    divisors[dcnt++] = (seq_t)j;
            }
        }

        /* Build the sequence: 42 - d (if >0) and 42 + d, plus the special 42. */
        sz = 0;
        for (idx_t i = 0; i < dcnt; ++i) {
            seq_t d = divisors[i];
            if (d < 42) {
                vals[sz++] = (seq_t)(42 - d);
            }
            vals[sz++] = (seq_t)(42 + d);
        }
        vals[sz++] = (seq_t)42;          /* Include k = 42 explicitly. */

        /* Sort the values to get the OEIS order. */
        qsort(vals, sz, sizeof(seq_t), cmp);

        initialized = true;
    }

    /* Return the nth term (1‑based), or 0 if out of bounds. */
    return (n > 0 && n <= sz) ? vals[n - 1] : (seq_t)0;
}
