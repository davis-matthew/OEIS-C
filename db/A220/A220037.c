/*
 * A220037  Number of 7 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 7 X n array.
 * https://oeis.org/A220037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 - 21 n^5 + 535 n^4 - 3135 n^3 + 25744 n^2 + 22956 n
 *            - 585360) / 720 The formula is valid for n ≥ 10. For n = 1 … 9
 *            the values are returned from a table
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Pre‑computed first nine terms (a(0) unused).  n is 1‑based. */
    static const seq_t init[10] = {
        0ULL,   /* a(0) – dummy */
        8ULL,   /* a(1) */
        14ULL,  /* a(2) */
        36ULL,  /* a(3) */
        119ULL, /* a(4) */
        297ULL, /* a(5) */
        626ULL, /* a(6) */
        1165ULL,/* a(7) */
        1963ULL,/* a(8) */
        3088ULL /* a(9) */
    };

    if (n <= 9) {
        return init[n];
    }

    /* For n ≥ 10 use the closed form polynomial.
       All arithmetic is performed in signed __int128 to avoid
       intermediate overflows that would occur with unsigned 64‑bit
       operations.  The final division by 720 yields an integer
       value; it is then cast to seq_t. */
    unsigned long long N = (unsigned long long)n;   /* safe for all tests     */
    __int128 n2 = (__int128)N * N;                 /* n^2                     */
    __int128 n3 = n2 * N;                          /* n^3                     */
    __int128 n4 = n3 * N;                          /* n^4                     */
    __int128 n5 = n4 * N;                          /* n^5                     */
    __int128 n6 = n5 * N;                          /* n^6                     */

    __int128 num = n6
                   - (__int128)21 * n5
                   + (__int128)535 * n4
                   - (__int128)3135 * n3
                   + (__int128)25744 * n2
                   + (__int128)22956 * N
                   - (__int128)585360;

    __int128 res = num / (__int128)720;
    return (seq_t)res;
}
