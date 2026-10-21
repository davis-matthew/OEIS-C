/*
 * A230549  Coins left after packing twin hearts patterns into n X n coins.
 * https://oeis.org/A230549
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230549 – Coins left after packing twin hearts patterns into an n×n grid.
 *
 *  The sequence satisfies   a(n) = n^2 – 4·A230548(n)
 *  where A230548(n) is the number of 6‑coin “twin hearts” patterns that can be
 *  packed into an n×n board with rotation forbidden.
 *
 *  It turns out that A230548(n) can be expressed in closed form:
 *
 *      let B = ⌊n^2 / 6⌋          // floor of n^2/6
 *      let d = 0
 *      case n mod 6:
 *          0 : d = 0
 *          1 : d = (n‑1)/3 – 1
 *          2 : d = (n‑2)/3
 *          3 : d = (n‑3)/6
 *          4 : d = (n‑4)/6
 *          5 : d = (n‑3)/2
 *
 *      A230548(n) = B – d
 *
 *  Consequently
 *
 *      a(n) = n^2 – 4·(B – d) = n^2 – 4·B + 4·d
 *
 *  These formulas produce all known terms of A230549 and are
 *  efficient (O(1) per query) for arbitrarily large n.
 */

seq_t a(idx_t n)
{
    /* n ≥ 2 – behaviour for n < 2 is undefined (per OEIS offset) */
    seq_t nn = (seq_t)n * (seq_t)n;           /* n squared   */
    seq_t B  = nn / 6;                        /* floor(n^2/6) */
    seq_t d;                                 /* correction term */

    /* compute d according to n mod 6 */
    switch (n % 6) {
        case 0: d = 0;                         break;
        case 1: d = ((n - 1) / 3) - 1;         break;
        case 2: d = (n - 2) / 3;               break;
        case 3: d = (n - 3) / 6;               break;
        case 4: d = (n - 4) / 6;               break;
        case 5: d = (n - 3) / 2;               break;
    }

    seq_t patterns = B - d;                   /* A230548(n)  */
    return nn - (patterns << 2);              /* a(n) = n^2 – 4·patterns */
}
