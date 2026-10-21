/*
 * A058003  Number of 5 X 5 matrices with entries mod n, up to row and column
 *          permutation.
 * https://oeis.org/A058003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/14400)*(n^25 + 20*n^20 + 100*n^17 + 70*n^15 + 300*n^14
 *            + 225*n^13 + 400*n^12 + 400*n^11 + 100*n^10 + 1600*n^9 +
 *            2300*n^8 + 1300*n^7 + 1200*n^6 + 1824*n^5 + 480*n^4 + 1680*n^3 +
 *            2400*n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* term coefficients and exponents */
    struct term { int coeff; int exp; };
    static const struct term terms[] = {
        { 1, 25 }, { 20, 20 }, { 100, 17 }, { 70, 15 }, { 300, 14 },
        { 225, 13 }, { 400, 12 }, { 400, 11 }, { 100, 10 }, { 1600, 9 },
        { 2300, 8 }, { 1300, 7 }, { 1200, 6 }, { 1824, 5 }, { 480, 4 },
        { 1680, 3 }, { 2400, 2 }
    };
    enum { NUM_TERMS = sizeof(terms)/sizeof(terms[0]) };

    /* If seq_t is 64‑bit or smaller, use 128‑bit intermediates to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 powers[26];
        powers[0] = (unsigned __int128)1;
        unsigned __int128 u = (unsigned __int128)n;
        for (int i = 1; i <= 25; ++i) powers[i] = powers[i-1] * u;

        unsigned __int128 sum = 0;
        for (int i = 0; i < NUM_TERMS; ++i)
            sum += (unsigned __int128)terms[i].coeff * powers[terms[i].exp];

        sum /= 14400;          /* exact division */
        return (seq_t)sum;
    }

    /* For larger seq_t, compute directly using seq_t arithmetic */
    seq_t powers[26];
    powers[0] = (seq_t)1;
    seq_t ns = (seq_t)n;
    for (int i = 1; i <= 25; ++i) powers[i] = powers[i-1] * ns;

    seq_t sum = (seq_t)0;
    for (int i = 0; i < NUM_TERMS; ++i)
        sum += (seq_t)terms[i].coeff * powers[terms[i].exp];

    sum /= (seq_t)14400;       /* exact division */
    return sum;
}
