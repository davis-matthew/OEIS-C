/*
 * A018348  Divisors of 236.
 * https://oeis.org/A018348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018348: sorted divisors of 236.
   The function enumerates all divisors of 236, sorts them,
   and returns the n‑th element (1‑based). For n beyond 6 it returns 0.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static void compute_divisors(seq_t *out, size_t *cnt)
{
    /* 236 = 2^2 * 59^1  ->  (exponents: 0..2 for 2, 0..1 for 59)  */
    size_t pos = 0;
    for (idx_t e2 = 0; e2 <= 2; ++e2) {
        seq_t mult2 = (seq_t)1 << e2;                /* 2^e2 */
        for (idx_t e59 = 0; e59 <= 1; ++e59) {
            seq_t mult59 = (e59 ? (seq_t)59 : (seq_t)1);  /* 59^e59 */
            out[pos++] = mult2 * mult59;
        }
    }
    *cnt = pos;

    /* simple insertion sort (array size is 6) */
    for (size_t i = 0; i < *cnt; ++i)
        for (size_t j = i + 1; j < *cnt; ++j)
            if (out[j] < out[i]) {
                seq_t tmp = out[i];
                out[i] = out[j];
                out[j] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    static bool ready = false;
    static seq_t divisor[6];
    static size_t divcnt = 0;

    if (!ready) {
        compute_divisors(divisor, &divcnt);
        ready = true;
    }

    if ((size_t)(n - 1) < divcnt)
        return divisor[n - 1];
    return 0;   /* no further terms */
}
