/*
 * A228825  Delayed continued fraction of e.
 * https://oeis.org/A228825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for OEIS A228825 (Delayed continued fraction of e):
 * a(0) = a(1) = 2.
 * After the initial two terms the sequence consists of cycles of four
 * blocks:
 *   B: three -1's
 *   C_k: odd‑length block 4*k+3 of (-2, 2) starting with -2
 *   D: three 1's
 *   E_k: odd‑length block 4*k+5 of ( 2, -2) starting with 2
 * For k = 0,1,2,… the blocks repeat with increasing odd lengths.
 * Example: [2,2, -1,-1,-1, -2,2,-2, 1,1,1, 2,-2,2,-2,2, ...]
 */

seq_t a(idx_t n)
{
    /* first two terms */
    if (n <= 1)
        return (seq_t)2;

    /* position inside the repeating blocks (index 2 is the first -1) */
    idx_t pos = n - 2;
    idx_t k = 0;

    while (1) {
        /* Block B: three -1's */
        if (pos < 3)
            return (seq_t)-1;
        pos -= 3;

        /* Block C_k: length 4*k+3, alternating -2, 2 */
        idx_t lenC = 4 * k + 3;
        if (pos < lenC)
            return (pos & 1) ? (seq_t)2 : (seq_t)-2;
        pos -= lenC;

        /* Block D: three 1's */
        if (pos < 3)
            return (seq_t)1;
        pos -= 3;

        /* Block E_k: length 4*k+5, alternating 2, -2 */
        idx_t lenE = 4 * k + 5;
        if (pos < lenE)
            return (pos & 1) ? (seq_t)-2 : (seq_t)2;
        pos -= lenE;

        /* next cycle */
        ++k;
    }
}
