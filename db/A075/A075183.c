/*
 * A075183  One half of third column of triangle A075181.
 * https://oeis.org/A075183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! * S1(n+3, n+1) / 2 * where S1(m, k) is the
 *            signless Stirling number of the first kind. * Since k = m-2, we
 *            use the recurrence * c_m = S1(m, m-2) = (m-1)^2 * (m-2) / 2 +
 *            c_{m-1} * with c_2 = 0. Then a(n) = (n+1)! * c_{n+3} / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t _c_size = 0;
static seq_t *_c_values = NULL;

static void _ensure_c(idx_t m)
{
    if (_c_size > m)
        return;
    idx_t new_size = (_c_size == 0) ? 8 : _c_size * 2;
    if (new_size <= m)
        new_size = m + 1;
    seq_t *tmp = (seq_t *)realloc(_c_values, new_size * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – caller may handle as zero */
    _c_values = tmp;
    /* initialize new entries if necessary */
    for (idx_t i = _c_size; i < new_size; ++i)
        _c_values[i] = 0;
    _c_size = new_size;
    /* fill in the recurrence for new indices */
    for (idx_t i = _c_size - new_size; i < new_size; ++i) {
        if (i >= 3) {
            seq_t term = (seq_t)(i - 1) * (seq_t)(i - 1)
                         * (seq_t)(i - 2) / 2;
            _c_values[i] = term + _c_values[i - 1];
        }
    }
}

seq_t a(idx_t n)
{
    /* factorial of (n+1) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fact *= (seq_t)i;

    /* ensure the array of c-values is large enough */
    idx_t m = n + 3;          /* we need S1(m, m-2) */
    _ensure_c(m);
    seq_t c_val = _c_values[m];

    /* compute fact * c_val / 2 without overflow when possible */
    seq_t result;
    if ((c_val & 1) == 0)
        result = fact * (c_val >> 1);
    else
        result = (fact >> 1) * c_val;

    return result;
}
