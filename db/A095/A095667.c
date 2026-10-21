/*
 * A095667  Fifth column of the (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+16) * binomial(n+3, 3) / 4
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 561, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  binomial(n+3, 3) = (n+3)(n+2)(n+1) / 6 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediates to avoid overflow on 32‑bit builds. */
    unsigned __int128 t0 = (unsigned __int128)(n + 3);
    unsigned __int128 t1 = (unsigned __int128)(n + 2);
    unsigned __int128 t2 = (unsigned __int128)(n + 1);
    unsigned __int128 num   = t0 * t1 * t2;      /* (n+3)(n+2)(n+1) */
    unsigned __int128 comb  = num / 6;          /* binomial(n+3,3)  */
    unsigned __int128 res   = ((unsigned __int128)(n + 16) * comb) / 4;
    return (seq_t)res;
}
