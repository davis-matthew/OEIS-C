/*
 * A079001  Digital equivalents of letters A, B, C, ..., Z on touch-tone
 *          telephone keypad.
 * https://oeis.org/A079001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the touch‑tone keypad digit for the * nth letter of the
 *            alphabet (A=1, B=2, …, Z=26), with the * mapping: * ABC → 2, DEF
 *            → 3, GHI → 4, JKL → 5, MNO → 6, * PQRS → 7, TUV → 8, WXYZ → 9. *
 *            The sequence repeats every 26 terms
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 26‑letter alphabet mapping */
    static const seq_t map[26] =
    {
        2, 2, 2,           /* A, B, C */
        3, 3, 3,           /* D, E, F */
        4, 4, 4,           /* G, H, I */
        5, 5, 5,           /* J, K, L */
        6, 6, 6,           /* M, N, O */
        7, 7, 7, 7,        /* P, Q, R, S */
        8, 8, 8,           /* T, U, V */
        9, 9, 9, 9         /* W, X, Y, Z */
    };

    /* n is 1‑based; convert to 0‑based index modulo 26 */
    idx_t idx = (n - 1) % 26;
    return map[idx];
}
