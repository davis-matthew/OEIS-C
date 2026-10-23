/*
 * A106261  Expansion of 1/sqrt(1 - 20*x - 20*x^2).
 * https://oeis.org/A106261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(2k, k) * C(k, n-k) * 5^k Recurrence:
 *            a(0)=1; a(1)=10; for n≥2 a(n) =
 *            ((20*n-10)*a(n-1)+(20*n-20)*a(n-2))/n The implementation uses a
 *            simple recurrence with memoisation. To avoid overflow when seq_t
 *            is a 64‑bit unsigned integer, 128‑bit intermediate arithmetic is
 *            employed
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for memory allocation and abort. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Static memoisation table. */
    static seq_t *table = NULL;   /* table[i] holds a(i) */
    static idx_t  last_index = 1; /* largest index already computed */
    static size_t capacity   = 2; /* current allocated size */

    /* Initialise the table on first call. */
    if (table == NULL) {
        table = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!table) abort();
        table[0] = 1;
        table[1] = 10;
    }

    /* Expand the table if necessary. */
    if (n > last_index) {
        if ((size_t)(n + 1) > capacity) {
            size_t newcap = n + 1;
            seq_t *tmp = (seq_t *)realloc(table, newcap * sizeof(seq_t));
            if (!tmp) abort();
            table = tmp;
            capacity = newcap;
        }

        /* Compute all missing terms using the recurrence. */
        for (idx_t i = last_index + 1; i <= n; ++i) {
            /* Use 128‑bit arithmetic when seq_t is 64 bits to avoid overflow. */
#ifdef __SIZEOF_INT128__
            if (sizeof(seq_t) == 8) {
                unsigned __int128 m1 = (unsigned __int128)20 * (unsigned __int128)i - 10;
                unsigned __int128 m2 = (unsigned __int128)20 * (unsigned __int128)i - 20;
                unsigned __int128 t1 = (unsigned __int128)table[i - 1] * m1;
                unsigned __int128 t2 = (unsigned __int128)table[i - 2] * m2;
                unsigned __int128 sum = t1 + t2;
                table[i] = (seq_t)(sum / (unsigned __int128)i);
            } else
#endif
            {
                seq_t m1 = (seq_t)20 * i - 10;
                seq_t m2 = (seq_t)20 * i - 20;
                seq_t t1 = table[i - 1] * m1;
                seq_t t2 = table[i - 2] * m2;
                table[i] = (t1 + t2) / i;
            }
        }
        last_index = n;
    }

    return table[n];
}
