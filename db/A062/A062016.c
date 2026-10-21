/*
 * A062016  Numbers with all odd digits which can be factored into two numbers
 *          > 1 with odd digits.
 * https://oeis.org/A062016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_odd_digits, grow_seq_array, has_odd_factorization, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062016 – Numbers with all odd digits that can be factored into two
   factors >1, each having only odd digits.  The function returns the
   n‑th term (1‑based).  Terms are generated on demand and cached in a
   static dynamic array that grows with the helper grow_seq_array. */

/* Include helpers from the utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"
#include "../utils/all_odd_digits.h"
#include "../utils/has_odd_factorization.h"
#include "../utils/isqrt.h"

/* --------------------------------------------------------------- */
/* Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t   *terms   = NULL;   /* cached terms */
    static size_t  count   = 0;       /* number of stored terms */
    static size_t  capacity = 0;      /* allocated space for terms */
    static unsigned long long cur = 1;/* last tested integer */

    if (n == 0) return 0;             /* undefined, but harmless */

    /* Generate terms until we have at least n of them */
    while (count < n) {
        ++cur;
        if (!all_odd_digits(cur)) continue;
        if (!has_odd_factorization(cur)) continue;

        /* Grow the array if full */
        if (count == capacity) {
            size_t newcap = capacity ? capacity << 1 : 16;
            grow_seq_array(&terms, &capacity, newcap);
        }
        terms[count++] = (seq_t)cur;
    }

    return terms[n - 1];
}
