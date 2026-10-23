/*
 * A011676  A binary m-sequence: expansion of reciprocal of x^7+x^6+x^4+x^2+1.
 * https://oeis.org/A011676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A011676 (binary m‑sequence from 1/(x^7+x^6+x^4+x^2+1) over GF(2))
   Recurrence: a(n) = a(n-2) XOR a(n-4) XOR a(n-6) XOR a(n-7)   for n ≥ 7
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1
   Period = 127. This implementation precomputes one period and
   returns the appropriate term using modulo arithmetic. */
seq_t a(idx_t n)
{
    /* static storage for one period of 127 bits */
    static seq_t period[127];
    static int initialized = 0;

    if (!initialized) {
        /* set the 7 known initial terms */
        period[0] = 0;
        period[1] = 0;
        period[2] = 0;
        period[3] = 0;
        period[4] = 0;
        period[5] = 0;
        period[6] = 1;
        /* fill the rest of the period using the recurrence */
        for (idx_t i = 7; i < 127; ++i) {
            period[i] =
                period[i-2] ^ period[i-4] ^ period[i-6] ^ period[i-7];
        }
        initialized = 1;
    }

    /* use modulo 127 to obtain the period index */
    return period[n % 127ULL];
}
