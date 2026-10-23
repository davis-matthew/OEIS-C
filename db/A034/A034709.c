/*
 * A034709  Numbers divisible by their last digit.
 * https://oeis.org/A034709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034709: Numbers divisible by their last decimal digit.
   The sequence is periodic with period 1177; each full period adds 2520
   to the previous term.  The first 1177 terms are pre‑computed and
   stored in a static array.  This implementation works for any
   integer type seq_t (uint8_t … uint64_t, uint128_t, big integer, …)
   and any idx_t (unsigned 64‑bit).  It is safe for all n ≥ 1 that
   produce a result fitting in seq_t.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BASE_SIZE 1177

/* cache for the first 1177 terms */
static seq_t *base = NULL;      /* lazy allocation */
static bool base_ready = false;

static void init_base(void)
{
    if (base_ready) return;

    base = (seq_t *)malloc(sizeof(seq_t) * BASE_SIZE);
    if (!base) return;          /* allocation failure: leave function dead‑end */

    idx_t cur  = 1;             /* candidate numbers */
    idx_t cnt  = 0;             /* number stored so far */

    while (cnt < BASE_SIZE) {
        uint64_t last = cur % 10;                 /* last decimal digit */
        if (last != 0 && cur % last == 0)         /* divisible by its last digit */
            base[cnt++] = (seq_t)cur;             /* store the value */
        cur++;
    }
    base_ready = true;
}

seq_t a(idx_t n)
{
    if (!base_ready) init_base();   /* build cache on first use */

    idx_t idx = n - 1;              /* convert to zero‑based index */
    idx_t k   = idx / BASE_SIZE;    /* number of full 1177‑periods */
    idx_t i   = idx % BASE_SIZE;    /* position within the period */

    /* a(k*1177 + i) = 2520*k + a(i) */
    seq_t result = base[i] + (seq_t)(2520 * k);
    return result;
}
