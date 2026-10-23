/*
 * A166253  String substitution 0 -> 01110, 1 -> 10001, started with 1.
 * https://oeis.org/A166253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Function: a
 *
 * Computes the nth term of OEIS sequence A166253.
 *
 * This sequence is the fixed point of the 5‑uniform morphism
 * defined by
 *
 *     0 → 01110
 *     1 → 10001
 *
 * starting with the single symbol "1".  The function returns the
 * (n‑th) symbol of this infinite word.  The algorithm
 * interprets the index n-1 in base 5 and follows the corresponding
 * transitions through the morphism tree to obtain the symbol.
 *
 * The implementation uses only integer arithmetic; it works for
 * any sequence length that fits into the provided integer types
 * seq_t and idx_t.
 */
seq_t a(idx_t n)
{
    /* n is 1‑based; if n == 0 the behaviour is undefined, return 0 */
    if (n == 0) return 0;

    /* Zero‑based index */
    idx_t pos = n - 1;

    /* Current symbol: start with the root symbol '1' */
    seq_t cur = 1;

    /* Transition tables for the morphism */
    static const seq_t trans[2][5] = {
        /* 0 → 0 1 1 1 0 */
        { 0, 1, 1, 1, 0 },
        /* 1 → 1 0 0 0 1 */
        { 1, 0, 0, 0, 1 }
    };

    /* Find the highest power of 5 not exceeding pos */
    idx_t pow5 = 1;
    while (pow5 <= pos / 5) pow5 *= 5;

    /* Walk through the base‑5 digits from most significant to least */
    while (pow5 > 0) {
        idx_t digit = pos / pow5;          /* digit in [0,4] */
        pos   %= pow5;                    /* remaining suffix */
        pow5   = pow5 / 5;                /* next lower power */

        /* Transition according to the current symbol and digit */
        cur = trans[(int)cur][(int)digit];
    }

    return cur;
}
