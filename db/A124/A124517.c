/*
 * A124517  Order of letters on an American keyboard reading top to bottom,
 *          left to right.
 * https://oeis.org/A124517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = the alphabetical index of the n-th letter in QWERTY
   keyboard order (read columns top‑to‑bottom, left‑to‑right),
   repeated every 26 positions.  */

/* Include only what is strictly needed */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against a zero index – not part of the sequence */
    if (n == 0)
        return 0;

    /* The first 26 letters of the standard QWERTY keyboard
       correspond to the 3 rows of the keyboard, read by
       columns from left to right, top to bottom.  */
    static const char row0[] = "qwertyuiop"; /* 10 letters */
    static const char row1[] = "asdfghjkl";   /*  9 letters */
    static const char row2[] = "zxcvbnm";     /*  7 letters */

    /* n is 1‑based; convert to 0‑based and wrap after 26 """
       This makes the sequence periodic. */
    idx_t idx = (n - 1) % (idx_t)26;   /* 0 ≤ idx ≤ 25 */

    /* Find the column and row within that column that
       contains the idx‑th letter.  */
    int col = 0;   /* column index (0 ≤ col ≤ 9) */
    int row = 0;   /* 0: top, 1: middle, 2: bottom */

    /* Remaining index in the current column */
    idx_t rem = idx;

    for (col = 0; ; ++col) {
        int height;
        if (col < 7)          height = 3;          /* columns 0–6 have three rows */
        else if (col == 7 || col == 8) height = 2;    /* columns 7–8 have two rows */
        else                    height = 1;          /* column 9 has one row */

        if (rem < (idx_t)height) {
            row = (int)rem;
            break;
        }
        rem -= (idx_t)height;
    }

    /* Determine the character in that (col,row) position. */
    char ch;
    if (row == 0) {
        ch = row0[col];
    } else if (row == 1) {
        /* row 1 exists for columns 0–8 */
        ch = row1[col];
    } else { /* row == 2 */
        /* row 2 exists only for columns 0–6 */
        ch = row2[col];
    }

    /* Convert the lowercase letter to its alphabetical index (1 – 26). */
    return (seq_t)(ch - 'a' + 1);
}
