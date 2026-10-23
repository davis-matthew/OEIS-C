/*
 * A153254  Three-digit numbers with sequential digits that can be displayed
 *          on a 12-hour digital clock.
 * https://oeis.org/A153254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the n-th 3‑digit number abc such that a, b, c are
 *            consecutive (|b-a|=|c-b|=1), the minutes part 10·b + c is ≤ 59,
 *            listed in increasing numeric order. Undefined for n beyond the
 *            finite set
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1 */

    static seq_t *terms = NULL;
    static size_t term_count = 0;

    if (terms == NULL)
    {
        /* pre‑compute all terms once */
        size_t cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (terms == NULL) return (seq_t)0; /* allocation failure */

        for (int num = 100; num <= 999; ++num)
        {
            int a = num / 100;
            int b = (num / 10) % 10;
            int c = num % 10;

            /* consecutive digits with step 1 */
            if (!(((b == a + 1) && (c == b + 1)) ||
                  ((b == a - 1) && (c == b - 1))))
                continue;

            /* minutes must not exceed 59 */
            int minutes = b * 10 + c;
            if (minutes > 59)
                continue;

            if (term_count == cap)
            {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (tmp == NULL)
                {
                    free(terms);
                    return (seq_t)0;
                }
                terms = tmp;
            }
            terms[term_count++] = (seq_t)num;
        }
    }

    if (n <= (idx_t)term_count)
        return terms[n - 1];
    return (seq_t)0;  /* value undefined for n beyond the finite list */
}
