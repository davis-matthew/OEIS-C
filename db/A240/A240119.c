/*
 * A240119  Schoenheim lower bound L(n,6,3).
 * https://oeis.org/A240119
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( n/6 * ceil( (n-1)/5 * ceil( (n-2)/4 ) ) )
 * Range:     u8 n <= 18, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the Schönheim lower bound L(n,6,3) for covering designs. */

seq_t a(idx_t n) {
    /* The formula is undefined for n < 6; return 0 in that case. */
    if (n < 6) return 0;

    /* Step 1: ceil((n-2)/4) */
    seq_t t1 = ((seq_t)(n - 2) + 3) / 4;

    /* Step 2: ceil((n-1)/5 * t1) == ceil(((n-1)*t1)/5) */
    seq_t prod2 = (seq_t)(n - 1) * t1;
    seq_t t2 = (prod2 + 4) / 5;

    /* Step 3: ceil(n/6 * t2) == ceil((n*t2)/6) */
    seq_t prod3 = (seq_t)n * t2;
    seq_t t3 = (prod3 + 5) / 6;

    return t3;
}
