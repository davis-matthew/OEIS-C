/*
 * A028447  Number of perfect matchings in graph P_{2} X P_{3} X P_{n}.
 * https://oeis.org/A028447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6a(n-1) + 21a(n-2) - 42a(n-3) - 89a(n-4) + 68a(n-5) +
 *            89a(n-6) - 42a(n-7) - 21a(n-8) + 6a(n-9) + a(n-10) a(0) = 1,
 *            a(1) = 3, a(2) = 32, a(3) = 229, a(4) = 1845, a(5) = 14320, a(6)
 *            = 112485, a(7) = 880163, a(8) = 6895792, a(9) = 54003765
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 10 terms known */
    static const seq_t init[10] = {
        1ULL,          /* a(0) */
        3ULL,          /* a(1) */
        32ULL,         /* a(2) */
        229ULL,        /* a(3) */
        1845ULL,       /* a(4) */
        14320ULL,      /* a(5) */
        112485ULL,     /* a(6) */
        880163ULL,     /* a(7) */
        6895792ULL,    /* a(8) */
        54003765ULL    /* a(9) */
    };

    if (n < 10)
        return init[n];

    /* keep the last 10 values while iterating */
    seq_t a0 = init[0], a1 = init[1], a2 = init[2], a3 = init[3],
          a4 = init[4], a5 = init[5], a6 = init[6], a7 = init[7],
          a8 = init[8], a9 = init[9];

    for (idx_t i = 10; i <= n; ++i) {
        seq_t next = 0;
        next += 6 * a9;
        next += 21 * a8;
        next -= 42 * a7;
        next -= 89 * a6;
        next += 68 * a5;
        next += 89 * a4;
        next -= 42 * a3;
        next -= 21 * a2;
        next += 6 * a1;
        next += a0;

        /* shift the window */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6;
        a6 = a7; a7 = a8; a8 = a9; a9 = next;
    }

    return a9;
}
