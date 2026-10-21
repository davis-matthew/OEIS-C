/*
 * A211772  Nonprime numbers all of whose divisors are numbers whose decimal
 *          digits are in ascending order.
 * https://oeis.org/A211772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is defined by the search of all non‑prime numbers whose
 *            every divisor has decimal digits in strictly ascending order
 *            (OEIS A211772). The function builds the full finite list on
 *            first use and returns the n‑th term
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   ascending_digits, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t); */
#include "../utils/ascending_digits.h"

/* Build the full list of terms up to the last known value
   134 689 (the sequence is finite).  The array is allocated
   once and kept in static storage for later calls. */
static void init_sequence(seq_t **seq_ptr, size_t *len_ptr)
{
    const idx_t LIMIT = 200000;     /* comfortably exceeds the last term */

    /* Pre‑compute which numbers ≤ LIMIT have ascending digits. */
    bool *asc = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    asc[0] = false;
    for (idx_t i = 1; i <= LIMIT; ++i)
        asc[i] = ascending_digits((seq_t)i);

    /* Storage for the resulting sequence; 63 terms, but give more space. */
    size_t cap  = 128;
    seq_t *arr  = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t cnt  = 0;

    for (idx_t m = 1; m <= LIMIT; ++m)
    {
        if (!asc[m])                /* the number itself must have ascending digits */
            continue;

        if (m > 1 && is_prime(m))   /* exclude primes (1 is allowed) */
            continue;

        bool ok = true;
        for (idx_t d = 2; d * d <= m && ok; ++d)
        {
            if (m % d == 0)
            {
                idx_t q = m / d;
                if (!asc[d] || (! (d == q) && !asc[q]))
                    ok = false;
            }
        }

        if (ok)
        {
            if (cnt == cap)       /* grow the array if needed */
            {
                cap <<= 1;
                arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
            }
            arr[cnt++] = (seq_t)m;
        }
    }

    free(asc);
    *seq_ptr = arr;
    *len_ptr = cnt;
}

seq_t a(idx_t n)
{
    static bool     initialized = false;
    static seq_t  *sequence    = NULL;
    static size_t   seq_len    = 0;

    if (!initialized)
    {
        init_sequence(&sequence, &seq_len);
        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;
    return sequence[n - 1];
}
