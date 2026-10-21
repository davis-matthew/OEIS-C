/*
 * A084631  a(n) = floor(C(n+8,8)/C(n+2,2)).
 * https://oeis.org/A084631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( binom(n+8,8) / binom(n+2,2) ) Derived polynomial:
 *            a(n) = 1 + floor( n*(n+11)*(n^4 +22*n^3 +203*n^2 +902*n +2232) /
 *            20160 )
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 204, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate when seq_t is 64‑bit, otherwise use seq_t itself.
       This guarantees that for n up to at least 10 000 the intermediate does not
       overflow and the result is exact. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = n;
        unsigned __int128 n2 = nn * nn;
        unsigned __int128 n3 = n2 * nn;
        unsigned __int128 n4 = n3 * nn;
        unsigned __int128 poly = n4 + 22 * n3 + 203 * n2 + 902 * nn + 2232;
        unsigned __int128 num  = nn * (nn + 11) * poly;
        unsigned __int128 res  = num / 20160u + 1u;
        return (seq_t)res;
    } else {
        seq_t tn = n;
        seq_t t1 = tn * (tn + 11);
        seq_t n2 = tn * tn;
        seq_t n3 = n2 * tn;
        seq_t n4 = n3 * tn;
        seq_t poly = n4 + 22 * n3 + 203 * n2 + 902 * tn + 2232;
        seq_t num  = t1 * poly;
        seq_t res  = num / (seq_t)20160 + 1;
        return res;
    }
}
