/*
 * A190796  Number of digits in the minimal base-phi representation of n.
 * https://oeis.org/A190796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of digits in the minimal base‑phi
 *            representation of n. Implemented via Lucas numbers: let L(0)=2,
 *            L(1)=1, L(n)=L(n-1)+L(n-2). If L(2k) ≤ n ≤ L(2k+1) then
 *            a(n)=4k+1; if L(2k+1)+1 ≤ n < L(2k+2) then a(n)=4k+4
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1)=1, a(0) is not defined in the OEIS offset */
        return (seq_t)1;

    idx_t e   = 2;      /* L[2k]   */
    idx_t o   = 1;      /* L[2k+1] */
    idx_t nxt = 3;      /* L[2k+2] */
    int k = 0;

    while (1) {
        if (n >= e && n <= o)
            return (seq_t)(4 * k + 1);

        if (n >= o + 1 && n < nxt)
            return (seq_t)(4 * k + 4);

        /* Advance to the next pair of Lucas numbers.
           New e = old nxt (L[2k+2]).
           New o = old nxt + old o (L[2k+3]).
           New nxt = new e + new o (L[2k+4]). */
        idx_t e_next   = nxt;
        idx_t o_next   = nxt + o;
        idx_t nxt_next = e_next + o_next;

        e   = e_next;
        o   = o_next;
        nxt = nxt_next;
        ++k;
    }
}
