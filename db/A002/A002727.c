/*
 * A002727  Number of 3 X n binary matrices up to row and column permutations.
 * https://oeis.org/A002727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) =
 *            4*a(n-1)-4*a(n-2)-2*a(n-3)+2*a(n-4)+4*a(n-5)+3*a(n-6)-12*a(n-7)+3*a(n-8)+4*a(n-9)+2*a(n-10)-2*a(n-11)-4*a(n-12)+4*a(n-13)-a(n-14)
 *            for n>13
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t initial[14] = {
        1, 4, 13, 36, 87, 190, 386, 734, 1324, 2284, 3790, 6080, 9473, 14378
    };
    
    if (n < 14)
        return initial[n];
    
    seq_t v[14];
    for (int i = 0; i < 14; i++)
        v[i] = initial[i];
    
    for (idx_t i = 14; i <= n; i++) {
        seq_t next = (seq_t)4 * v[13]
                   - (seq_t)4 * v[12]
                   - (seq_t)2 * v[11]
                   + (seq_t)2 * v[10]
                   + (seq_t)4 * v[9]
                   + (seq_t)3 * v[8]
                   - (seq_t)12 * v[7]
                   + (seq_t)3 * v[6]
                   + (seq_t)4 * v[5]
                   + (seq_t)2 * v[4]
                   - (seq_t)2 * v[3]
                   - (seq_t)4 * v[2]
                   + (seq_t)4 * v[1]
                   - v[0];
        
        for (int j = 0; j < 13; j++)
            v[j] = v[j + 1];
        v[13] = next;
    }
    
    return v[13];
}
