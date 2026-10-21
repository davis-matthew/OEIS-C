/*
 * A067521  Numbers n such that the square root of n is an integer and a
 *          multiple of the sum of the digits of n.
 * https://oeis.org/A067521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 2876, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_uint128, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by checking successive squares: a(n) is the n‑th square m^2
 * where m is a multiple of the sum of the decimal digits of m^2. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"
#include "../utils/digit_sum_uint128.h"

static size_t terms_size = 0;     /* current capacity of the terms array   */
static seq_t *terms = NULL;       /* stored terms a(1), a(2), ...           */
static idx_t computed = 0;        /* number of terms already computed      */
static uint64_t m_current = 1;    /* next integer m to test                */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but harmless */

    /* extend precomputation until we have at least n terms */
    while (computed < n) {
        __int128 sq = (__int128)m_current * m_current;
        uint64_t sum = digit_sum_uint128(sq);

        /* sum will be >0 for m>0; check multiplicity condition */
        if (sum > 0 && (m_current % sum) == 0) {
            /* ensure space for new term */
            if (computed >= terms_size)
                grow_seq_array(&terms, &terms_size, computed + 1);

            terms[computed] = (seq_t)sq;
            computed++;
        }

        m_current++;
    }

    return terms[n - 1];
}
