/*
 * A106693  Limiting word of the morphism 1->1132311, 2->2213122, 3->3321233
 *          starting from 1.
 * https://oeis.org/A106693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106693: a(n) = the letter obtained by iteratively applying the morphism
 * 1→1132311, 2→2213122, 3→3321233 starting from the seed 1, selecting
 * subletters according to the base‑7 digits of the 0‑based index n. */
seq_t a(idx_t n) {
    /* Mapping from a current symbol to the 7 symbols it expands into. */
    static const int MAP[3][7] = {
        {1, 1, 3, 2, 3, 1, 1},   /* sigma(1) */
        {2, 2, 1, 3, 1, 2, 2},   /* sigma(2) */
        {3, 3, 2, 1, 2, 3, 3}    /* sigma(3) */
    };

    int cur = 1;                  /* start from the seed symbol 1 */

    while (n > 0) {
        idx_t d = n % (idx_t)7;   /* current base‑7 digit (0‑6) */
        n /= (idx_t)7;
        cur = MAP[cur - 1][d];    /* choose the d‑th subletter */
    }

    return (seq_t)cur;           /* convert to seq_t for the return type */
}
