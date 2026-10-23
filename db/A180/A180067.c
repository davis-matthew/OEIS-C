/*
 * A180067  Number of ways to place 9 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A180067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^16 - 324*n^14 + 46914*n^12 - 3975048*n^10 * +
 *            216203169*n^8 - 7756575876*n^6 + 179987135516*n^4 * -
 *            2481599151792*n^2 + 15651056776320) / 362880 * Offset: 1 (a(1)
 *            is the first term listed)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 9, u64 n <= 24, u128 n <= 280,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Detect support for the unsigned 128‑bit type */
#if defined(__SIZEOF_INT128__) || defined(__INT128_TYPE__) || defined(__UINT128_TYPE__)
#define HAVE_U128 1
#else
#define HAVE_U128 0
#endif

/* Use 128‑bit intermediate only when the seq_t type is small enough */
static const int use_u128 = HAVE_U128 && (sizeof(seq_t) <= 8);

seq_t a(idx_t n)
{
    /* Small values for 1 ≤ n ≤ 9 */
    if (n <= 9)
    {
        static const seq_t tbl[9] = {
            (seq_t)0,
            (seq_t)0,
            (seq_t)0,
            (seq_t)0,
            (seq_t)0,
            (seq_t)28,
            (seq_t)81095,
            (seq_t)42752576ULL,
            (seq_t)2436444603ULL
        };
        return tbl[n - 1];
    }

    /* For n ≥ 10 use the polynomial formula */
    if (use_u128)
    {
        __uint128_t N = n;
        __uint128_t n2 = N * N;
        __uint128_t n4 = n2 * n2;
        __uint128_t n6 = n4 * n2;
        __uint128_t n8 = n4 * n4;
        __uint128_t n10 = n8 * n2;
        __uint128_t n12 = n8 * n4;
        __uint128_t n14 = n12 * n2;
        __uint128_t n16 = n8 * n8;

        __uint128_t poly = n16
            - (__uint128_t)324 * n14
            + (__uint128_t)46914 * n12
            - (__uint128_t)3975048 * n10
            + (__uint128_t)216203169 * n8
            - (__uint128_t)7756575876 * n6
            + (__uint128_t)179987135516 * n4
            - (__uint128_t)2481599151792ULL * n2
            + (__uint128_t)15651056776320ULL;

        __uint128_t num = n2 * poly;
        seq_t result = (seq_t)(num / 362880ULL);
        return result;
    }
    else
    {
        /* Arbitrary‑precision or 64‑bit seq_t that cannot use __uint128_t */
        seq_t n2 = (seq_t)n * (seq_t)n;
        seq_t n4 = n2 * n2;
        seq_t n6 = n4 * n2;
        seq_t n8 = n4 * n4;
        seq_t n10 = n8 * n2;
        seq_t n12 = n8 * n4;
        seq_t n14 = n12 * n2;
        seq_t n16 = n8 * n8;

        seq_t poly = n16
            - (seq_t)324 * n14
            + (seq_t)46914 * n12
            - (seq_t)3975048 * n10
            + (seq_t)216203169 * n8
            - (seq_t)7756575876 * n6
            + (seq_t)179987135516 * n4
            - (seq_t)2481599151792ULL * n2
            + (seq_t)15651056776320ULL;

        seq_t num = n2 * poly;
        seq_t result = num / (seq_t)362880ULL;
        return result;
    }
}
