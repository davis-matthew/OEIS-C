/*
 * A105789  Fixed point of the morphism 1 -> 12321, 2 -> 43234, 3 -> 21412, 4
 *          -> 34143, starting from a(0) = 1.
 * https://oeis.org/A105789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th symbol of the infinite word fixed by the
 *            morphism: 1 -> 1 2 3 2 1 2 -> 4 3 2 3 4 3 -> 2 1 4 1 2 4 -> 3 4
 *            1 4 3 starting from 1 (a(0)=1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t M[5][5] = {
    /* row 0 – unused */      {0,0,0,0,0},
    /* row 1 */                {1,2,3,2,1},
    /* row 2 */                {4,3,2,3,4},
    /* row 3 */                {2,1,4,1,2},
    /* row 4 */                {3,4,1,4,3}
};

static seq_t get_letter(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t parent = n / 5;
    idx_t pos    = n % 5;
    seq_t p_sym  = get_letter(parent);
    return M[(int)p_sym][(int)pos];
}

seq_t a(idx_t n)
{
    return get_letter(n);
}
