/*
 * A110003  n followed by n^3 followed by n^2 followed by n^4.
 * https://oeis.org/A110003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b^e , where b = (n-1)/4 + 1 (block base) r = (n-1) % 4
 *            (position in the block) exp e = 1 if r==0, 3 if r==1, 2 if r==2,
 *            4 if r==3
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t block   = (n - 1) / 4;          /* 0‑based block index   */
    seq_t base    = (seq_t)block + 1;     /* base = block+1       */
    int r         = (int)((n - 1) % 4);   /* position inside block */
    int exp;

    switch (r) {
        case 0: exp = 1; break;
        case 1: exp = 3; break;
        case 2: exp = 2; break;
        default: exp = 4; break;          /* r==3 */
    }

    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;

    return result;
}
