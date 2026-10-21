/*
 * A085801  Maximum number of nonattacking queens on an n X n toroidal board.
 * https://oeis.org/A085801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n ≡ 1,5,7,11 (mod 12); a(n) = n-1 for n ≡ 2,10 (mod
 *            12); otherwise a(n) = n-2
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 as safety */

    idx_t mod = n % (idx_t)12;

    /* values mod 12 that return n */
    if (mod == 1 || mod == 5 || mod == 7 || mod == 11)
        return (seq_t)n;

    /* values mod 12 that return n-1 */
    if (mod == 2 || mod == 10)
        return (seq_t)n - (seq_t)1;

    /* all other residues: return n-2 */
    return (seq_t)n - (seq_t)2;
}
