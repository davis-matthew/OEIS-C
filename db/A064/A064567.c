/*
 * A064567  Reciprocal of n terminates with an infinite repetition of digit 8.
 *          Multiples of 10 are omitted.
 * https://oeis.org/A064567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest number in the set
 *            {72·64^(k−1)}∪{1125·15625^(k−1)}. These are the values
 *            9·2^(6i−3) and 9·5^(6i−3) for i>0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(1)=72, a(2)=1125, a(3)=4608, ...  */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t a_val = (seq_t)72;    /* 9·2^(6·1−3)  */
    seq_t b_val = (seq_t)1125;  /* 9·5^(6·1−3)  */

    for (;;) {
        seq_t next = (a_val <= b_val) ? a_val : b_val;
        ++count;

        if (count == n)
            return next;

        if (a_val <= b_val)
            a_val *= (seq_t)64;      /* multiply by 2^6 */
        else
            b_val *= (seq_t)15625;   /* multiply by 5^6 */
    }
}
