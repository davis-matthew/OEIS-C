/*
 * A210589  Numbers which, when divided by their first digit, have their last
 *          digit as remainder.
 * https://oeis.org/A210589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers which, when divided by their first digit, have their last digit as remainder. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *terms = NULL;
    static size_t count   = 0;
    static size_t capacity = 0;
    static seq_t last_checked = 9; /* start before the first term (10) */

    if ((size_t)n <= count)
        return terms[n - 1];

    while (count < (size_t)n)
    {
        last_checked++;                    /* next candidate integer */
        seq_t num = last_checked;

        seq_t last_digit = num % 10;
        seq_t tmp = num;
        seq_t first_digit = tmp;
        while (first_digit >= 10)
            first_digit /= 10;

        if (num % first_digit == last_digit)
        {
            if (count == capacity)
            {
                size_t new_cap = capacity == 0 ? 8 : capacity << 1;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) return 0;   /* allocation failure: return 0 as placeholder */
                terms = new_terms;
                capacity = new_cap;
            }
            terms[count++] = num;
        }
    }
    return terms[n - 1];
}
