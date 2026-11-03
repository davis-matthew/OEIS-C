/*
 * A086761  Numbers k such that k-th cyclotomic polynomial has exactly 5
 *          nonzero terms.
 * https://oeis.org/A086761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086761: numbers k such that the k‑th cyclotomic polynomial has exactly 5 nonzero terms.
   The terms are all numbers of the form 2^a·5^b with a≥0, b>0, sorted in increasing order.
   This implementation generates the list of 2^a·5^b numbers in ascending order and
   records those that are divisible by 5 (i.e., b≥1).  The nth such number is returned. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

static seq_t *h = NULL;        /* all numbers 2^a·5^b in increasing order, starting with 1 */
static size_t hcap = 0, hlen = 0; /* capacity and current length of h            */
static idx_t i2 = 0, i5 = 0;        /* indices for next multiples of 2 and 5     */

static seq_t *divs = NULL;     /* numbers divisible by 5 (the sequence values) */
static size_t dcap = 0, dlen = 0; /* capacity and current length of divs           */

#ifdef __cplusplus
template <class T>
static void grow(T **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 16 : *cap << 1;
    if (newcap < needed)
        newcap = needed;
    *ptr = mem_resize(T, *ptr, (newcap * elem_size) / sizeof(T));
    *cap = newcap;
}
#else
static void grow(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 16 : *cap << 1;
    if (newcap < needed)
        newcap = needed;
    *ptr = realloc(*ptr, newcap * elem_size);
    *cap = newcap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n < 1, return 0 as sentinel */
        return 0;

    size_t target = (size_t)n;      /* we need the target‑th element (1‑based) */

    /* --- initialise containers on first call -------------------------------- */
    if (h == NULL) {
        grow(OEISC_VPP(&h), &hcap, 1, sizeof(seq_t));
        h[0] = (seq_t)1;           /* 1 = 2^0·5^0, the starting point */
        hlen = 1;
        i2 = i5 = 0;
    }
    if (divs == NULL) {
        grow(OEISC_VPP(&divs), &dcap, 1, sizeof(seq_t));
        dlen = 0;
    }

    /* --- generate numbers until we have the requested amount of divs --------*/
    while (dlen < target) {
        /* ensure space for next Hamming number */
        grow(OEISC_VPP(&h), &hcap, hlen + 1, sizeof(seq_t));

        seq_t next2 = h[i2] * (seq_t)2;
        seq_t next5 = h[i5] * (seq_t)5;
        seq_t next = (next2 < next5) ? next2 : next5;
        h[hlen++] = next;               /* store the new number */

        if ((next % (seq_t)5) == 0) {    /* store if divisible by 5 */
            grow(OEISC_VPP(&divs), &dcap, dlen + 1, sizeof(seq_t));
            divs[dlen++] = next;
        }

        if (next == next2) i2++;
        if (next == next5) i5++;
    }

    return divs[target - 1];
}
