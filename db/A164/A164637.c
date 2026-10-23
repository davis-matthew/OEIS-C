/*
 * A164637  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=24, a(2)=552, a(3)=12696, a(4)=292008,
 *            a(5)=6716184, a(6)=154472232, a(7)=3552861060; for n>=8: a(n) =
 *            22*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 253*a(n-7) (the
 *            recurrence follows from the rational generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[8] = {
    (seq_t)1,          /* a(0) */
    (seq_t)24,         /* a(1) */
    (seq_t)552,        /* a(2) */
    (seq_t)12696,      /* a(3) */
    (seq_t)292008,     /* a(4) */
    (seq_t)6716184,    /* a(5) */
    (seq_t)154472232,  /* a(6) */
    (seq_t)3552861060  /* a(7) */
};

/* Numerator coefficients for n=0..7:
   N(0)=1, N(1..6)=2, N(7)=1.  For n>7, N(n)=0. */
static const seq_t num_coeff[8] = {
    (seq_t)1,
    (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
    (seq_t)1
};

seq_t a(idx_t n)
{
    if (n <= 7) {
        return init[n];
    }

    /* Keep the last 7 values in a circular buffer. */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) {
        buf[i] = init[i];
    }
    int pos = 6; /* index of a(n-1) (currently a(6)) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute sum of the previous 6 terms. */
        seq_t sum6 = buf[pos];
        sum6 += buf[(pos + 6) % 7]; /* a(n-2) */
        sum6 += buf[(pos + 5) % 7]; /* a(n-3) */
        sum6 += buf[(pos + 4) % 7]; /* a(n-4) */
        sum6 += buf[(pos + 3) % 7]; /* a(n-5) */
        sum6 += buf[(pos + 2) % 7]; /* a(n-6) */

        seq_t term7 = buf[(pos + 1) % 7]; /* a(n-7) */

        /* Recurrence: a(i) = 22*sum6 - 253*term7 + N(i) (for i<=7) */
        seq_t next = sum6 * (seq_t)22 - term7 * (seq_t)253;
        if (i <= 7) {
            next += num_coeff[i];
        }

        /* Advance buffer: position for a(i) */
        pos = (pos + 1) % 7;
        buf[pos] = next;
    }

    return buf[pos];
}
