/*
 * A085239  Sort the numbers 2^i and 3^j. Then a(n) is the base of the n-th
 *          term. Set a(1)=1.
 * https://oeis.org/A085239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the base (2 or 3, except a(1)=1) of the n‑th element
 *            when the two sequences 2^i (i≥0) and 3^j (j≥0) are merged and
 *            sorted, with the repeated element 1 kept only once
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Offset: a(1) = 1. */
    if (n == 1) return 1;

    /* We merge the two geometric progressions using their logarithms
       to avoid overflow of the actual power values. */
    const double LOG2 = 0.6931471805599453094172321214581765680755; /* ln 2 */
    const double LOG3 = 1.098612288668109691395245236922525704647; /* ln 3 */

    idx_t i = 1;            /* exponent for 2^i (starts at 1) */
    idx_t j = 1;            /* exponent for 3^j (starts at 1) */
    double log2 = LOG2;     /* ln(2^i) */
    double log3 = LOG3;     /* ln(3^j) */

    idx_t terms = 1;        /* count the initial 1 */

    while (terms < n) {
        if (log2 < log3) {          /* next term comes from the 2‑sequence */
            ++terms;
            if (terms == n) return 2;
            ++i;
            log2 += LOG2;
        } else {                    /* next term comes from the 3‑sequence */
            ++terms;
            if (terms == n) return 3;
            ++j;
            log3 += LOG3;
        }
    }

    /* The code logic guarantees we return inside the loop. */
    return 0; /* unreachable, but keeps compiler happy */
}
