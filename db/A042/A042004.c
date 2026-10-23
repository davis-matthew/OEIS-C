/*
 * A042004  Numerators of continued fraction convergents to sqrt(525).
 * https://oeis.org/A042004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042004:  Numerators of continued fraction convergents to sqrt(525)
   a(0)=22, a(1)=23, a(2)=252, a(3)=527, a(4)=5522, a(5)=6049
   Recurrence: a(n) = 12098*a(n-6) - a(n-12)  for n >= 12
*/
seq_t a(idx_t n)
{
    /* initial 12 values */
    static const seq_t init_vals[12] = {
        (seq_t) 22LLU, (seq_t) 23LLU, (seq_t) 252LLU, (seq_t) 527LLU,
        (seq_t)5522LLU, (seq_t)6049LLU, (seq_t)271678LLU, (seq_t)277727LLU,
        (seq_t)3048948LLU, (seq_t)6375623LLU, (seq_t)66805178LLU, (seq_t)73180801LLU
    };

    if (n < 12)
        return init_vals[(int)n];

    /* ring buffer of the last 12 computed terms */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i) buf[i] = init_vals[i];

    for (idx_t k = 12; k <= n; ++k) {
        int idx6  = (int)((k - 6) % 12);
        int idx12 = (int)((k - 12) % 12);
        seq_t next = ((seq_t)12098 * buf[idx6]) - buf[idx12];
        buf[(int)(k % 12)] = next;
    }

    return buf[(int)(n % 12)];
}
