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
 * Validated: standard, pass, 52 terms through n = 9406 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by checking successive squares: a(n) is the n‑th square m^2
 * where m is a multiple of the sum of the decimal digits of m^2. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* helper: digit sum of a non‑negative __int128 number */
static uint64_t digit_sum_uint128(__int128 x)
{
    uint64_t sum = 0;
    while (x > 0) {
        sum += (uint64_t)(x % 10);
        x /= 10;
    }
    return sum;
}

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
