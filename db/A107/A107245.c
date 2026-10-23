/*
 * A107245  Sum of squares of heptanacci numbers (Fibonacci 7-step numbers).
 * https://oeis.org/A107245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} F_7(k)^2 where F_7 is the 7‑step Fibonacci
 *            (heptanacci) sequence with
 *            F_7(0)=F_7(1)=F_7(2)=F_7(3)=F_7(4)=F_7(5)=0, F_7(6)=1 and for
 *            k≥7, F_7(k)=F_7(k−1)+…+F_7(k−7)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* buffer holding the last seven heptanacci values;
       initially they represent F_7(0)…F_7(6) */
    seq_t f[7] = {0, 0, 0, 0, 0, 0, 1};

    for (idx_t i = 0; i <= n; ++i) {
        seq_t val;

        if (i < 7) {
            /* For indices 0…6 the values are already stored in f */
            val = f[i];
        } else {
            /* Compute F_7(i) as the sum of the previous seven values */
            val = 0;
            for (int k = 0; k < 7; ++k)
                val += f[k];

            /* Shift the buffer left to make room for the new value */
            for (int k = 0; k < 6; ++k)
                f[k] = f[k + 1];
            f[6] = val;
        }

        /* Add square of the current term to the running sum */
        sum += val * val;
    }

    return sum;
}
