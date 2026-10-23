/*
 * A048997  Sum of digits in decimal expansion of 1/n up to the point where it
 *          repeats for the first time.
 * https://oeis.org/A048997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Sum of digits of the decimal expansion of 1/n up to the first
 *            digit that repeats. The non‑repeating part plus the digits of
 *            the first full cycle are summed
 * Range:     u8 n <= 58, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Allocate an array to remember the first position of each remainder.
       The remainder can be at most n-1; we use n+1 to keep indices simple. */
    int32_t *seen = (int32_t*)malloc((size_t)(n + 1) * sizeof(int32_t));
    if (!seen)
        return (seq_t)0;                           /* allocation failure */
    for (idx_t i = 0; i <= n; ++i)
        seen[i] = -1;

    /* Dynamically growable array of digits. */
    unsigned char *digits = NULL;
    size_t cap = 0, len = 0;

    idx_t rem = 1 % n;          /* initial remainder after integer part 0 */
    idx_t pos = 0;              /* position of the current digit */

    /* Simulate long division until remainder repeats or becomes zero. */
    while (1) {
        if (rem == 0)                /* terminating decimal */
            break;
        if (seen[rem] != -1)         /* cycle detected */
            break;

        seen[rem] = (int32_t)pos;    /* remember position of this remainder */

        /* Compute the next digit: floor((rem * 10) / n).  Use 128‑bit
           arithmetic to avoid overflow for large n. */
        idx_t digit = (idx_t)(((unsigned __int128)rem * 10u) / n);

        /* Store the digit. */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            unsigned char *tmp = (unsigned char*)realloc(digits, new_cap);
            if (!tmp) { free(digits); free(seen); return (seq_t)0; }
            digits = tmp;
            cap = new_cap;
        }
        digits[len++] = (unsigned char)digit;

        /* Prepare next remainder. */
        rem = (idx_t)(((unsigned __int128)rem * 10u) % n);
        ++pos;
    }

    /* Compute sums. */
    seq_t sum_nonrep = 0, sum_cycle = 0;
    if (rem == 0) {                     /* terminating */
        for (size_t i = 0; i < len; ++i)
            sum_nonrep += digits[i];
    } else {                            /* repeating */
        int32_t cycle_start = seen[rem];
        for (int32_t i = 0; i < cycle_start; ++i)
            sum_nonrep += digits[i];
        for (int32_t i = cycle_start; i < (int32_t)len; ++i)
            sum_cycle += digits[i];
    }

    seq_t result = sum_nonrep + sum_cycle;

    free(digits);
    free(seen);
    return result;
}
