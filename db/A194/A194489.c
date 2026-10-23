/*
 * A194489  Number of ways to arrange 5 nonattacking knights on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3840)*(n^10 + 5n^9 - 170n^8 + 90n^7 + 14265n^6 * -
 *            68939n^5 - 458560n^4 + 4600940n^3 * - 2393296n^2 - 91938176n +
 *            278112000) * for n > 10, with a(1) … a(10) given below
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 21, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for 1 ≤ n ≤ 10 */
    static const seq_t SMALL[11] = {
        0,          /* unused */
        0,          /* a(1) */
        0,          /* a(2) */
        0,          /* a(3) */
        13,         /* a(4) */
        387,        /* a(5) */
        3727,       /* a(6) */
        23496,      /* a(7) */
        111433,     /* a(8) */
        429343,     /* a(9) */
        1407755     /* a(10) */
    };

    if (n <= 10)
        return SMALL[n];

    /* For small seq_t (≤ 64‑bit) use 128‑bit arithmetic for safety. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow[11];
        pow[0] = (unsigned __int128)1;
        for (idx_t i = 1; i <= 10; ++i)
            pow[i] = pow[i - 1] * (unsigned __int128)n;

        unsigned __int128 pos = pow[10];
        pos += (unsigned __int128)5   * pow[9];
        pos += (unsigned __int128)90  * pow[7];
        pos += (unsigned __int128)14265 * pow[6];
        pos += (unsigned __int128)4600940 * pow[3];
        pos += (unsigned __int128)278112000;

        unsigned __int128 neg = (unsigned __int128)170  * pow[8];
        neg += (unsigned __int128)68939 * pow[5];
        neg += (unsigned __int128)458560 * pow[4];
        neg += (unsigned __int128)2393296 * pow[2];
        neg += (unsigned __int128)91938176 * pow[1];

        unsigned __int128 numer = pos - neg;
        return (seq_t)(numer / (unsigned __int128)3840);
    }

    /* seq_t is arbitrary‑precision or ≥ 128 bits */
    seq_t pow[11];
    pow[0] = (seq_t)1;
    for (idx_t i = 1; i <= 10; ++i)
        pow[i] = pow[i - 1] * (seq_t)n;

    seq_t pos = pow[10];
    pos += (seq_t)5   * pow[9];
    pos += (seq_t)90  * pow[7];
    pos += (seq_t)14265 * pow[6];
    pos += (seq_t)4600940 * pow[3];
    pos += (seq_t)278112000;

    seq_t neg = (seq_t)170  * pow[8];
    neg += (seq_t)68939 * pow[5];
    neg += (seq_t)458560 * pow[4];
    neg += (seq_t)2393296 * pow[2];
    neg += (seq_t)91938176 * pow[1];

    seq_t numer = pos - neg;
    return numer / (seq_t)3840;
}
