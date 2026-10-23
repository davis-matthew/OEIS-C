/*
 * A165182  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=50, a(2)=2450, a(3)=120050,
   a(4)=5882450, a(5)=288240050, a(6)=14123762450,
   a(7)=692064360050, a(8)=33911153641225,
   a(n) = 48*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
          - 1176*a(n-8)  for n >= 9.
*/
seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[9] = {
        (seq_t)1ULL,
        (seq_t)50ULL,
        (seq_t)2450ULL,
        (seq_t)120050ULL,
        (seq_t)5882450ULL,
        (seq_t)288240050ULL,
        (seq_t)14123762450ULL,
        (seq_t)692064360050ULL,
        (seq_t)33911153641225ULL
    };

    if (n < 9)
        return init[n];

    idx_t i;
    seq_t last[8];
    /* initialise last[0..7] with a(1)..a(8) */
    for (i = 0; i < 8; ++i)
        last[i] = init[i + 1];

    seq_t next;
    for (i = 9; i <= n; ++i) {
        /* sum a(i-1) ... a(i-7) : last[1]..last[7] */
        seq_t sum = last[1];
        sum += last[2];
        sum += last[3];
        sum += last[4];
        sum += last[5];
        sum += last[6];
        sum += last[7];

        /* recurrence */
        next = ((seq_t)48) * sum - ((seq_t)1176) * last[0];

        /* shift the window: drop a(i-8), add new a(i) */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = next;
    }
    return last[7];
}
