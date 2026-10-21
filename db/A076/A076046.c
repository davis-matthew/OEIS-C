/*
 * A076046  Ramanujan-Nagell numbers: the triangular numbers (of the form
 *          a*(a+1)/2) which are also of the form 2^b - 1.
 * https://oeis.org/A076046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the nth triangular number such that t+1 is a power of
 *            two. Search: generate triangular numbers t = a*(a+1)/2 and check
 *            if t+1 is 2^k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   _is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/_is_power_of_two.h"

/* Temporary storage for the sequence terms.
   The sequence is finite (5 terms in the OEIS entry). */
static seq_t _terms[5];
static size_t _term_count = 0;

/* Initialise the stored terms on first use. */
static void _init_terms(void)
{
    if (_term_count > 0)          /* already initialised */
        return;

    /* Upper bound large enough to capture all known terms. */
    const idx_t MAX_M = 100000;
    for (idx_t m = 0; m <= MAX_M && _term_count < 5; ++m)
    {
        idx_t tri   = m * (m + 1) / 2;   /* triangular number */
        idx_t plus1 = tri + 1;
        seq_t val   = (seq_t)plus1;       /* cast for the power‑of‑two test */

        if (_is_power_of_two(val))
            _terms[_term_count++] = (seq_t)tri;
    }
}

/* External‑linkage function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1)                     /* offset is 1; below offset is irrelevant */
        return 0;

    _init_terms();

    if (n <= (idx_t)_term_count)   /* return the stored term if it exists */
        return _terms[n - 1];
    else
        return 0;                  /* undefined terms are 0 */
}
