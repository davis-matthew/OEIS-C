/*
 * A084698  Beginning with 1 the smallest multiple of n such that the n-th
 *          partial sum is a square.
 * https://oeis.org/A084698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*n for n>16 and n odd, a(n) = 96*n for n>16 and n even,
 *            a(n) given by the following table for 1≤n≤16:
 *            {1,8,27,28,80,180,252,208,441,800,891,684,1300,2156,2160,784}
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[17] = {
        0ULL,   /* unused, index 0 */
        1ULL,   /* a(1) */
        8ULL,   /* a(2) */
        27ULL,  /* a(3) */
        28ULL,  /* a(4) */
        80ULL,  /* a(5) */
        180ULL, /* a(6) */
        252ULL, /* a(7) */
        208ULL, /* a(8) */
        441ULL, /* a(9) */
        800ULL, /* a(10) */
        891ULL, /* a(11) */
        684ULL, /* a(12) */
        1300ULL,/* a(13) */
        2156ULL,/* a(14) */
        2160ULL,/* a(15) */
        784ULL  /* a(16) */
    };

    if (n <= 16)
        return small[n];

    if (n & 1ULL)            /* n is odd */
        return (seq_t)n * (seq_t)48;
    else                     /* n is even */
        return (seq_t)n * (seq_t)96;
}
