/*
 * A050043  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A050043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(2*n-2-2^(p+1)), where p = floor_log2(n-2)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 127, u64 n <= 506,
 *            u128 n <= 3087, big any n
 * Validated: standard, pass, 59 terms through n = 7594 (2026-11-03)
 * Helpers:   ilog2, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1) = 1, a(2) = 2, a(3) = 1 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache --------------------------------------------------------*/
    static seq_t *buf = NULL;   /* indices 0..cap-1, 0 unused */
    static idx_t cap  = 0;      /* allocated capacity */
    static idx_t size = 0;      /* maximum index already filled */

    /* base cases ----------------------------------------------------------*/
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* ensure the cache can hold index n -------------------------------*/
    if (n >= cap) {
        /* grow with some slack; use calloc to clear new area */
        idx_t new_cap = n + 16;
        size_t new_bytes = (size_t)new_cap * sizeof(seq_t);
        seq_t *new_buf = mem_new(seq_t, (new_bytes) * (1) / sizeof(seq_t));
        if (!new_buf) abort();  /* irrecoverable for this program */

        if (buf) {
            mem_copy(new_buf, buf, cap * sizeof(seq_t));
            mem_free(buf);
        }
        buf = new_buf;
        cap = new_cap;
    }

    /* initialise the first three entries if not already done ----------*/
    if (size < 3) {
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)2;
        buf[3] = (seq_t)1;
        size   = 3;
    }

    /* compute missing terms up to n -------------------------------------*/
    for (idx_t i = size + 1; i <= n; ++i) {
        /* p = floor_log2(n-2) ------------------------------------------*/
        int p = (int)ilog2(i - 2);

        /* 2^(p+1) -------------------------------------------------------*/
        seq_t powp = pow2((idx_t)(p + 1));

        /* m = 2*i - 2 - 2^(p+1) ---------------------------------------*/
        seq_t two_i_minus2 = (seq_t)(2 * (i - 1));   /* 2*i-2 */
        idx_t m_idx = (idx_t)(two_i_minus2 - powp); /* m is < i */

        /* recurrence -----------------------------------------------------*/
        buf[i] = buf[i - 1] + buf[m_idx];
    }
    size = n;

    return buf[n];
}
