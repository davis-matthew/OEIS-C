/*
 * A094636  Largest digit of n in Roman numeral representation.
 * https://oeis.org/A094636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   largest Roman numeral digit of n. For n ≥ 1, we convert n to its
 *            Roman numeral representation using the standard greedy algorithm
 *            and return the value of the largest symbol that appears (I = 1,
 *            V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000). If n > 3999
 *            the representation continues with repea...
 * Range:     u8 n <= 399, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Roman numeral tables */
    static const idx_t     values[13]  = { 1000,  900, 500, 400, 100,  90,
                                           50,   40, 10,   9,   5,  4,   1 };
    static const seq_t     maxD[13]   = { 1000, 1000,500, 500, 100,  100,
                                           50,   50, 10,  10,   5,  5,   1 };

    if (n <= 0)            /* undefined for 0 or negative, but return 0 safely */
        return (seq_t)0;

    seq_t largest = 0;
    idx_t i = 0;

    while (n > 0) {
        while (n < values[i])  /* find largest value not exceeding n */
            ++i;

        /* use this value once */
        if (maxD[i] > largest)
            largest = maxD[i];
        n -= values[i];
    }
    return largest;
}
