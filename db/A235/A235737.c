/*
 * A235737  Number of (n+1)X(1+1) 0..2 arrays with the sum of each 2X2
 *          subblock maximum and minimum lexicographically nondecreasing
 *          columnwise and nonincreasing rowwise.
 * https://oeis.org/A235737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -144*a(n-3) +122*a(n-4) +726*a(n-5) -372*a(n-6)
 *            -1500*a(n-7) +239*a(n-8) +1353*a(n-9) +156*a(n-10) -444*a(n-11)
 *            -144*a(n-12) Initial values: a(1)=81, a(2)=576, a(3)=3992,
 *            a(4)=26088, a(5)=167892, a(6)=1060410, a(7)=6648825,
 *            a(8)=41411637, a(9)=257111073, a(10)=1592383950, a(11)=98...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values a(1)..a(12) */
    static const seq_t base[12] = {
        (seq_t)81ULL,
        (seq_t)576ULL,
        (seq_t)3992ULL,
        (seq_t)26088ULL,
        (seq_t)167892ULL,
        (seq_t)1060410ULL,
        (seq_t)6648825ULL,
        (seq_t)41411637ULL,
        (seq_t)257111073ULL,
        (seq_t)1592383950ULL,
        (seq_t)9849348220ULL,
        (seq_t)60864784416ULL
    };

    if (n <= 12)
        return base[n - 1];

    /* Keep the 12 most recent terms: last[0] = a(n-12), last[11] = a(n-1) */
    seq_t last[12];
    for (idx_t i = 0; i < 12; ++i)
        last[i] = base[i];

    for (idx_t i = 13; i <= n; ++i) {
        seq_t next = (seq_t)9 * last[11];
        next -= (seq_t)144 * last[9];
        next += (seq_t)122 * last[8];
        next += (seq_t)726 * last[7];
        next -= (seq_t)372 * last[6];
        next -= (seq_t)1500 * last[5];
        next += (seq_t)239 * last[4];
        next += (seq_t)1353 * last[3];
        next += (seq_t)156 * last[2];
        next -= (seq_t)444 * last[1];
        next -= (seq_t)144 * last[0];

        /* Shift buffer: remove oldest term, make room for new term */
        for (idx_t j = 0; j < 11; ++j)
            last[j] = last[j + 1];
        last[11] = next;
    }

    return last[11];
}
