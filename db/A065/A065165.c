/*
 * A065165  Permutation t->t+2 of Z, folded to N.
 * https://oeis.org/A065165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(-1)^n + n for n>3 (i.e. a(n) = n+4 if n even, a(n) =
 *            n-4 if n odd, for n≥4); a(1)=4, a(2)=6, a(3)=2
 * Range:     u8 n <= 251, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n <= 3) {
        switch (n) {
            case 1: return 4;
            case 2: return 6;
            case 3: return 2;
        }
    }

    /* For n ≥ 4 use the closed‑form expression */
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {       /* n is even */
        return sn + (seq_t)4;
    } else {                   /* n is odd */
        return sn - (seq_t)4;
    }
}
