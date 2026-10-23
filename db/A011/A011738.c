/*
 * A011738  A binary m-sequence: expansion of reciprocal of x^25 + x^3 + 1
 *          (mod 2, shifted by 24 initial 0's).
 * https://oeis.org/A011738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in x^24/(x^25 + x^3 + 1) over GF(2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation is via the recurrence  c(n) = c(n-3) XOR c(n-25)   (n ≥ 25) */
/* with c(0)=1, c(1)=c(2)=0.  Then  a(n)=0 for 0≤n<24 and a(n)=c(n-24) otherwise. */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *cbuf = NULL;                  /* storage for c(0)…c(PER-1) */
static const idx_t PER = ((idx_t)1ULL << 25) - 1ULL;   /* 2^25 – 1 */

/* Initialize the recurrence table once */
static void init_cbuf(void)
{
    cbuf = (uint8_t*)malloc(PER);
    if (!cbuf) abort();                      /* insufficient memory */

    cbuf[0] = 1;                             /* c(0) = 1 */

    for (idx_t i = 1; i < PER; ++i)
    {
        if (i < 3)                          /* n = 1,2   → 0 */
            cbuf[i] = 0;
        else if (i < 25)                    /* 3 ≤ n < 25 */
            cbuf[i] = cbuf[i - 3];
        else                                /* n ≥ 25   */
            cbuf[i] = cbuf[i - 3] ^ cbuf[i - 25];
    }
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    if (!cbuf) init_cbuf();                 /* lazy initialization */

    if (n < 24) return (seq_t)0;            /* initial 24 zeros */

    /* index into the periodic c array */
    idx_t k = (n - 24) % PER;
    return (seq_t)cbuf[k];
}
