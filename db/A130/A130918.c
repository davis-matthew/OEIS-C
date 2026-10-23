/*
 * A130918  Simple self-inverse permutation of natural numbers: List each
 *          block of A000108(n) numbers from A014137(n-1) to A014138(n-1) in
 *          reverse order.
 * https://oeis.org/A130918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by dividing n into blocks of length Catalan
 *            numbers * and listing each block in reverse order. The block
 *            that contains n is the * first k such that * * n <
 *            S(k)=∑_{i=0}^{k} C_i, * * where C_i is the i-th Catalan number.
 *            Let S(k-1)=cum be the start of that * block and C_k=cat its...
 * Range:     u8 n <= 196, u16 n <= 2055, u32 n <= 2055, u64 n <= 2055,
 *            u128 n <= 2055, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cat = 1;          /* C0 */
    seq_t cum = 0;          /* cumulative number before the current block */
    idx_t  t   = 0;         /* current block index */

    while (n >= cum + cat) {
        cum += cat;
        /* Catalan recurrence: C_{t+1} = C_t * (4*t + 2) / (t + 2) */
        cat = cat * ( ((seq_t)4 * (seq_t)t) + (seq_t)2 )
                 / ((seq_t)(t + 2));
        ++t;
    }

    idx_t offset = n - (idx_t)cum;
    return cum + cat - 1 - (seq_t)offset;
}
