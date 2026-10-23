/*
 * A042692  Numerators of continued fraction convergents to sqrt(876).
 * https://oeis.org/A042692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  p_{-2}=0, p_{-1}=1
   p_n = a_n * p_{n-1} + p_{n-2}
   where a_0 = 29 and for n ≥ 1, a_n repeats the pattern
   1, 1, 2, 14, 2, 1, 1, 58 (period 8).  The function a(n) returns
   p_n, the numerator of the n-th convergent to √876. */

static const seq_t cf[8] = {(seq_t)1,(seq_t)1,(seq_t)2,(seq_t)14,
                            (seq_t)2,(seq_t)1,(seq_t)1,(seq_t)58};

seq_t a(idx_t n)
{
    seq_t p_prevprev = (seq_t)0;   /* p_{-2} */
    seq_t p_prev    = (seq_t)1;   /* p_{-1} */
    seq_t p_curr;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i = (i == 0) ? (seq_t)29 : cf[(i - 1) % 8];
        p_curr = a_i * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_prev;  /* p_n */
}
