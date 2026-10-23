/*
 * A106689  Limiting word of the morphism 1->11321, 2->22312, 3->33213
 *          starting from 1.
 * https://oeis.org/A106689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=morphtab[ a(n/5) ][ n%5 ], where
 *            morphtab[1]={1,1,3,2,1}, morphtab[2]={2,2,3,1,2},
 *            morphtab[3]={3,3,2,1,3}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t morphtab[3][5] = {
    { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)2, (seq_t)1 },
    { (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)1, (seq_t)2 },
    { (seq_t)3, (seq_t)3, (seq_t)2, (seq_t)1, (seq_t)3 }
};

static seq_t a_rec(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t parent = n / 5;
    idx_t offset = n % 5;
    seq_t p_val = a_rec(parent);
    int row = (int)((unsigned long long)p_val) - 1;   /* map 1→0, 2→1, 3→2 */
    return morphtab[row][(int)offset];
}

seq_t a(idx_t n) {
    return a_rec(n);
}
