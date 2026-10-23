/*
 * A025949  Expansion of 1/((1-2*x)*(1-3*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A025949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1)-2^(n+1)+18*a(n-1)-80*a(n-2) a(0) = 1, a(1) = 23
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic, iterating
   from 0 up to the requested index. No static storage is needed
   and it works for any supported seq_t, whether 8‑, 16‑, 32‑,
   64‑, 128‑bit or arbitrary‑precision. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;

    /* a_prev2 = a(i-2), a_prev1 = a(i-1) */
    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev1 = (seq_t)23;         /* a(1) */

    /* pow2_base = 2^(i-1), pow3_base = 3^(i-1)  (starting with i = 2) */
    seq_t pow2_base = (seq_t)2;        /* 2^(1) */
    seq_t pow3_base = (seq_t)3;        /* 3^(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute 2^i and 3^i */
        seq_t pow2_next = pow2_base * (seq_t)2;   /* 2^i   */
        seq_t pow3_next = pow3_base * (seq_t)3;   /* 3^i   */

        /* Compute 2^(i+1) and 3^(i+1) */
        seq_t pow2_next_next = pow2_next * (seq_t)2;  /* 2^(i+1) */
        seq_t pow3_next_next = pow3_next * (seq_t)3;  /* 3^(i+1) */

        /* Apply the recurrence */
        seq_t val = pow3_next_next - pow2_next_next
                    + (seq_t)18 * a_prev1
                    - (seq_t)80 * a_prev2;

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = val;
        pow2_base = pow2_next;
        pow3_base = pow3_next;
    }

    return a_prev1;
}
