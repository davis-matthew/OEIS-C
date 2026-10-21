/*
 * A202296  The first a(n) positive multiples of n together include every
 *          digit.
 * https://oeis.org/A202296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 8570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the minimal number of positive multiples of n needed to see
   every digit 0–9 at least once in their decimal representations.  The
   function returns this minimal k. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* dep­endent on offset; not used */

    unsigned mask = 0;            /* bit i set iff digit i seen */
    const unsigned all = 0x3FFu;  /* 10 bits set */

    seq_t k = 0;                  /* counted multiples */
    for (unsigned i = 1; ; ++i) {
        seq_t value = (seq_t)n * i;   /* i-th multiple of n */

        /* iterate over decimal digits of value */
        seq_t t = value;
        if (t == 0)            /* zero contains digit 0 */
            mask |= 1u << 0;
        while (t > 0) {
            unsigned d = (unsigned)(t % 10u);
            mask |= 1u << d;
            t /= 10u;
        }

        if (mask == all) {        /* all digits seen */
            k = i;
            break;
        }
    }
    return k;
}
