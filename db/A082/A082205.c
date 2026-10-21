/*
 * A082205  Palindromes pertaining to A082204.
 * https://oeis.org/A082205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1001*a(n-3) - 1000*a(n-6) for n >= 8 * Initial values:
 *            a(1)=1, a(2)=22, a(3)=232, a(4)=3223, * a(5)=22322, a(6)=232232,
 *            a(7)=3223223
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly */
    if (n <= 7) {
        static const seq_t base[8] = {
            0ULL,           /* unused index 0 */
            1ULL, 22ULL, 232ULL, 3223ULL,
            22322ULL, 232232ULL, 3223223ULL
        };
        return base[n];
    }

    /* ring[0..5] holds a(i-6), a(i-5), a(i-4), a(i-3), a(i-2), a(i-1) for current i */
    seq_t ring[6];
    ring[0] = 22ULL;          /* a(2) */
    ring[1] = 232ULL;         /* a(3) */
    ring[2] = 3223ULL;        /* a(4) */
    ring[3] = 22322ULL;       /* a(5) */
    ring[4] = 232232ULL;      /* a(6) */
    ring[5] = 3223223ULL;     /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = 1001ULL * ring[3] - 1000ULL * ring[0];
        /* shift left to make room for next term */
        ring[0] = ring[1];
        ring[1] = ring[2];
        ring[2] = ring[3];
        ring[3] = ring[4];
        ring[4] = ring[5];
        ring[5] = next;
    }
    return ring[5];
}
