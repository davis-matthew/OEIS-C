/*
 * A208381  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A208381
    Number of 5×n 0..1 arrays avoiding 0 0 0 / 1 1 1 horizontally
    and 0 0 1 / 1 0 1 vertically.
    Recurrence (n > 8):
        a(n) = a(n-1) + 4*a(n-2) + 13*a(n-3) + 8*a(n-4)
               - 3*a(n-5) + 9*a(n-6)
    Initial values:
        a(1)=10, a(2)=100, a(3)=144, a(4)=640,
        a(5)=2176, a(6)=7072, a(7)=25200, a(8)=87040
*/

seq_t a(idx_t n)
{
    /* Handle the offset – a(1) corresponds to the first term */
    if (n == 0)
        return 0;        /* value not used by OEIS, placeholder */

    /* Initial terms – index 0 is unused to keep the indices 1..8 matching */
    static const seq_t base[9] = {
        0ULL,               /* 0 (unused)          */
        10ULL,              /* a(1)                */
        100ULL,             /* a(2)                */
        144ULL,             /* a(3)                */
        640ULL,             /* a(4)                */
        2176ULL,            /* a(5)                */
        7072ULL,            /* a(6)                */
        25200ULL,           /* a(7)                */
        87040ULL            /* a(8)                */
    };

    if (n <= 8)
        return base[n];

    /* Window of the last six values: a(n-1), a(n-2), …, a(n-6) */
    seq_t a1 = base[8];  /* a(n-1) */
    seq_t a2 = base[7];  /* a(n-2) */
    seq_t a3 = base[6];  /* a(n-3) */
    seq_t a4 = base[5];  /* a(n-4) */
    seq_t a5 = base[4];  /* a(n-5) */
    seq_t a6 = base[3];  /* a(n-6) */

    seq_t next = 0;

    for (idx_t i = 9; i <= n; ++i) {
        /* Compute next term with unsigned arithmetic */
        next = a1 + 4 * a2 + 13 * a3 + 8 * a4 + 9 * a6;
        next -= 3 * a5;          /* subtraction done after addition to keep unsigned operations */

        /* Shift the window for the next iteration */
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return next;
}
