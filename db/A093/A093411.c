/*
 * A093411  Divide n by the largest factorial that divides it and repeat until
 *          an odd number is reached, which will be the result; a(0) = 0.
 * https://oeis.org/A093411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 8850, u32 n <= 8850, u64 n <= 8850,
 *            u128 n <= 8850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093411: a(n) = n divided by the largest factorial that divides it,
 * repeated until the result is odd.  a(0)=0. */

/* The implementation repeatedly removes the greatest factorial factor from
 * n, always advancing to a smaller odd number, until an odd result is
 * reached.  Factorials are pre‑computed on‑demand in a static array.
 * Operations use only the arithmetic and comparison operators that are
 * supported for `seq_t`.  The routine works for the 64‑bit build as well
 * as for the arbitrary‑precision build.  The complexity is O(log n)
 * per call. */
seq_t a(idx_t n) {
    /* Handle the trivial case explicitly */
    if (n == 0) return (seq_t)0;

    /* Start with n as a seq_t value */
    seq_t val = (seq_t)n;

    /* If n is already odd, it is the answer. */
    if ((val & (seq_t)1) == (seq_t)1) return val;

    /* Static factorial table: fact[i] == i!  (i >= 0)  */
    static seq_t fact[128];
    static int fact_len = 0;   /* maximum i such that fact[i] is defined */

    /* Initialise the table once */
    if (fact_len == 0) {
        fact[0] = (seq_t)1;    /* 0! = 1, not used for divisibility */
        fact[1] = (seq_t)1;    /* 1! = 1 */
        fact_len = 1;
    }

    /* Main loop: repeatedly remove the largest factorial factor */
    while (1) {
        /* Ensure factorials are available up to the point where
         * the next factorial would be larger than the current val. */
        while (fact_len + 1 < 128 && fact[fact_len] <= val) {
            int next = fact_len + 1;
            fact[next] = fact[fact_len] * (seq_t)next;
            fact_len = next;
        }

        /* Find the largest factorial <= val that divides val */
        int i = fact_len;
        while (i > 1 && fact[i] > val) i--;   /* skip too large factorials */

        seq_t f = (seq_t)1;    /* fallback: no factorial divisor > 1 */
        for (; i >= 2; --i) {
            if ((val % fact[i]) == (seq_t)0) {
                f = fact[i];
                break;
            }
        }

        /* If no factorial divisor > 1 is found, val must be odd.
         * In any case, division by 1 would leave val unchanged,
         * so we stop to avoid an infinite loop. */
        if (f == (seq_t)1) break;

        /* Divide by the found factorial and repeat */
        val = val / f;

        /* Stop if the new value is odd */
        if ((val & (seq_t)1) == (seq_t)1) break;
    }

    return val;
}
